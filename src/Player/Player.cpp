#include "Player.h"
#include "TickManager.h"
#include "World/World.h"
#include "Main.h"

#define WALK_SPEED 1
#define RUN_SPEED 1.3
#define FLY_SPEED 5
#define SWIM_SPEED 0.02

extern int64_t nanotime;

namespace voxel
{

	Player::Player(World &world)
	: Entity(world, nullptr)
	, raycast(*this)
	, oldMouseX(0)
	, oldMouseY(0)
	, eyeInWater(false)
	{
		this->flying = true;
		setSize(Vec3(.6, 1.8, .6));
		setPos(Vec3(8, 150, 8));
	}

	void Player::handleMovement()
	{
		Vec3 add(0);
		bool keyW = Main::getWindow()->isKeyDown(GLFW_KEY_W);
		bool keyA = Main::getWindow()->isKeyDown(GLFW_KEY_A);
		bool keyS = Main::getWindow()->isKeyDown(GLFW_KEY_S);
		bool keyD = Main::getWindow()->isKeyDown(GLFW_KEY_D);
		bool keySpace = Main::getWindow()->isKeyDown(GLFW_KEY_SPACE);
		bool keyLShift = Main::getWindow()->isKeyDown(GLFW_KEY_LEFT_SHIFT);
		if (!keyW && !keyA && !keyS && !keyD && !keySpace && !keyLShift)
			return;
		if (keyW && keyS)
		{
			keyW = false;
			keyS = false;
		}
		if (keyA && keyD)
		{
			keyA = false;
			keyD = false;
		}
		if (keySpace && keyLShift)
		{
			keySpace = false;
			keyLShift = false;
		}
		if (keyW || keyA || keyS || keyD)
		{
			double angle = this->rot.y;
			if (keyW && keyD)
				angle -= 45;
			else if (keyW && keyA)
				angle -= 135;
			else if (keyW)
				angle -= 90;
			else if (keyS && keyA)
				angle += 135;
			else if (keyS && keyD)
				angle += 45;
			else if (keyS)
				angle += 90;
			else if (keyA)
				angle += 180;
			else if (keyD)
				angle += 0;
			add.x = std::cos(angle / 180. * M_PI);
			add.z = std::sin(angle / 180. * M_PI);
			if (this->flying)
			{
				add.x *= FLY_SPEED;
				add.z *= FLY_SPEED;
			}
			else if (this->inWater)
			{
				add.x *= SWIM_SPEED;
				add.z *= SWIM_SPEED;
			}
			else if (Main::getWindow()->isKeyDown(GLFW_KEY_LEFT_CONTROL))
			{
				add.x *= RUN_SPEED;
				add.z *= RUN_SPEED;
			}
			else
			{
				add.x *= WALK_SPEED;
				add.z *= WALK_SPEED;
			}
		}
		if (this->flying)
		{
			if (keySpace)
				add.y += FLY_SPEED;
			else if (keyLShift)
				add.y -= FLY_SPEED;
		}
		else
		{
			if (keySpace)
			{
				if (this->inWater)
					add.y += .04;
				else if (this->isOnFloor)
						jump();
			}
		}
		if (this->flying)
			add *= 0.1;
		else if (!this->inWater)
		{
			if (this->isOnFloor)
				add *= 0.1;
			else
				add *= 0.02;
		}
		this->posDst += add;
	}

	void Player::handleRotation()
	{
		int32_t mouseX = Main::getWindow()->getMouseX();
		int32_t mouseY = Main::getWindow()->getMouseY();
		if (mouseX == this->oldMouseX && mouseY == this->oldMouseY)
			return;
		this->rot.y += (mouseX - this->oldMouseX) / 20.;
		this->rot.x += (mouseY - this->oldMouseY) / 20.;
		this->oldMouseX = mouseX;
		this->oldMouseY = mouseY;
		this->rot.y = std::fmod(this->rot.y, 360);
		if (this->rot.y < 0)
			this->rot.y += 360;
		if (this->rot.x > 90)
			this->rot.x = 90;
		else if (this->rot.x < -90)
			this->rot.x = -90;
	}

	void Player::tick()
	{
		handleMovement();
		Entity::tick();
		this->raycast.tick();
	}

	void Player::draw()
	{
		this->raycast.draw();
	}

	void Player::update()
	{
		Vec3 pos(this->world.getPlayer().getPos());
		pos.y += .72 + 2. / 16;
		ChunkBlock *block = this->world.getBlock(pos.x, pos.y, pos.z);
		this->eyeInWater = block && (block->getType() == 8 || block->getType() == 9);
		this->eyeLight = this->world.getLight(pos.x, pos.y, pos.z);
		handleRotation();
		float fov = 90;
		if (this->eyeInWater)
			fov -= 10;
		if (Main::getWindow()->isKeyDown(GLFW_KEY_LEFT_CONTROL))
			fov += 10;
		this->projMat = Mat4::perspective(fov / 180. * M_PI, Main::getWindow()->getWidth() / static_cast<float>(Main::getWindow()->getHeight()), .05f, 1500.f);
		this->viewMat = Mat4(1.);
		//this->viewMat = Mat4::translate(this->viewMat, Vec3(std::cos(nanotime / 800000000. * M_PI * 2) * 0.02, 0, 0));
		//this->viewMat = Mat4::rotateZ(this->viewMat, std::pow(std::cos(nanotime / 1600000000. * M_PI * 2) * 2, 2) / 4 * 0.010);
		//this->viewMat = Mat4::rotateX(this->viewMat, std::pow(std::cos(nanotime /  800000000. * M_PI * 2) * 2, 2) / 4 * 0.005 + this->rot.x / 180. * M_PI);
		this->viewMat = Mat4::rotateX(this->viewMat, this->rot.x / 180. * M_PI);
		this->viewMat = Mat4::rotateY(this->viewMat, this->rot.y / 180. * M_PI);
		Vec3 realPos = getRealPos();
		this->viewMat = Mat4::translate(this->viewMat, Vec3(-realPos.x, -realPos.y - 0.72, -realPos.z));
		this->viewProjMat = this->projMat * this->viewMat;
		this->world.getFrustum().update(this->viewProjMat);
		this->raycast.raycast();
	}

}
