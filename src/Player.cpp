#include "Player.h"
#include "World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

#define WALK_SPEED 4.3
#define RUN_SPEED 5.6
#define FLY_SPEED 10

extern int64_t frameDelta;
extern int64_t nanotime;

namespace voxel
{

	Player::Player(World &world)
	: Entity(world)
	, raycast(*this)
	, oldMouseX(0)
	, oldMouseY(0)
	{
		this->flying = true;
		setSize(.6, 1.8, .6);
		setPos(0, 128, 0);
		this->projMat = glm::perspective(glm::radians(80.), Main::getWindow()->getWidth() / static_cast<double>(Main::getWindow()->getHeight()), .019, 1000.);
	}

	void Player::handleMovement()
	{
		float addX = 0;
		float addY = 0;
		float addZ = 0;
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
			addX = frameDelta / 1000000000. * std::cos(angle / 180. * M_PI);
			addZ = frameDelta / 1000000000. * std::sin(angle / 180. * M_PI);
			if (this->flying)
			{
				addX *= FLY_SPEED;
				addZ *= FLY_SPEED;
			}
			else if (Main::getWindow()->isKeyDown(GLFW_KEY_LEFT_CONTROL))
			{
				addX *= RUN_SPEED;
				addZ *= RUN_SPEED;
			}
			else
			{
				addX *= WALK_SPEED;
				addZ *= WALK_SPEED;
			}
		}
		if (this->flying)
		{
			if (keySpace)
				addY += frameDelta / 1000000000. * FLY_SPEED;
			else if (keyLShift)
				addY -= frameDelta / 1000000000. * FLY_SPEED;
		}
		else
		{
			if (this->isOnFloor)
			{
				if (keySpace)
					jump();
			}
		}
		move(addX, addY, addZ);
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
		Entity::tick();
		handleMovement();
		handleRotation();
		this->raycast.tick();
		this->viewMat = glm::mat4(1.);
		//this->viewMat = glm::translate(this->viewMat, glm::vec3(std::cos(nanotime / 800000000. * M_PI * 2) * 0.01, 0, 0));
		//this->viewMat = glm::rotate(this->viewMat, glm::vec2(std::pow(std::cos(nanotime / 1600000000. * M_PI * 2) * 2, 2) / 4 * 0.010, 0).x, glm::vec3(0, 0, 1));
		//this->viewMat = glm::rotate(this->viewMat, glm::vec2(std::pow(std::cos(nanotime /  800000000. * M_PI * 2) * 2, 2) / 4 * 0.005 + this->rotX / 180. * M_PI, 0).x, glm::vec3(1, 0, 0));
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rot.x / 180. * M_PI, 0).x, glm::vec3(1, 0, 0));
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rot.y / 180. * M_PI, 0).x, glm::vec3(0, 1, 0));
		this->viewMat = glm::translate(this->viewMat, glm::vec3(-this->pos.x, -this->pos.y - 0.72, -this->pos.z));
		this->world.getFrustum().update();
	}

	void Player::draw()
	{
		this->raycast.draw();
	}

}
