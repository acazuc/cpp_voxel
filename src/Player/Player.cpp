#include "Player.h"
#include "TickManager.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

#define WALK_SPEED 1
#define RUN_SPEED 1.3
#define FLY_SPEED 5
#define SWIM_SPEED 0.02

extern int64_t nanotime;

namespace voxel
{

	Player::Player(World &world)
	: Entity(world, NULL)
	, raycast(*this)
	, oldMouseX(0)
	, oldMouseY(0)
	, eyeInWater(false)
	{
		this->flying = true;
		setSize(glm::vec3(.6, 1.8, .6));
		setPos(glm::vec3(0, 150, 0));
	}

	void Player::handleMovement()
	{
		glm::vec3 add;
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
			if (this->inWater)
			{
				add.x *= SWIM_SPEED;
				add.z *= SWIM_SPEED;
			}
			else if (this->flying)
			{
				add.x *= FLY_SPEED;
				add.z *= FLY_SPEED;
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
		if (!this->inWater)
		{
			if (this->isOnFloor || this->flying)
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
		glm::vec3 pos(this->world.getPlayer().getPos());
		pos.y += .72 + 2. / 16;
		ChunkBlock *block = this->world.getBlock(pos);
		this->eyeInWater = block && (block->getType() == 8 || block->getType() == 9);
		this->eyeLight = this->world.getLight(pos);
		handleRotation();
		float fov = 90;
		if (this->eyeInWater)
			fov -= 10;
		if (Main::getWindow()->isKeyDown(GLFW_KEY_LEFT_CONTROL))
			fov += 10;
		this->projMat = glm::perspective(glm::radians(fov), Main::getWindow()->getWidth() / static_cast<float>(Main::getWindow()->getHeight()), .019f, 1500.f);
		this->viewMat = glm::mat4(1.);
		//this->viewMat = glm::translate(this->viewMat, glm::vec3(std::cos(nanotime / 800000000. * M_PI * 2) * 0.02, 0, 0));
		//this->viewMat = glm::rotate(this->viewMat, glm::vec2(std::pow(std::cos(nanotime / 1600000000. * M_PI * 2) * 2, 2) / 4 * 0.010, 0).x, glm::vec3(0, 0, 1));
		//this->viewMat = glm::rotate(this->viewMat, glm::vec2(std::pow(std::cos(nanotime /  800000000. * M_PI * 2) * 2, 2) / 4 * 0.005 + this->rot.x / 180. * M_PI, 0).x, glm::vec3(1, 0, 0));
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rot.x / 180. * M_PI, 0).x, glm::vec3(1, 0, 0));
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rot.y / 180. * M_PI, 0).x, glm::vec3(0, 1, 0));
		glm::vec3 realPos = getRealPos();
		this->viewMat = glm::translate(this->viewMat, glm::vec3(-realPos.x, -realPos.y - 0.72, -realPos.z));
		this->viewProjMat = this->projMat * this->viewMat;
		this->world.getFrustum().update(this->viewProjMat);
		this->raycast.raycast();
	}

}
