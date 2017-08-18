#include "Player.h"
#include "Main.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define MOVEMENT_SPEED 0.24

namespace voxel
{

	Player::Player()
	: posX(0)
	, posY(0)
	, posZ(0)
	, rotX(0)
	, rotY(0)
	, rotZ(0)
	{
		//Empty
	}

	bool Player::handleMovementXZ()
	{
		bool keyW = Main::getWindow()->isKeyDown(GLFW_KEY_W);
		bool keyA = Main::getWindow()->isKeyDown(GLFW_KEY_A);
		bool keyS = Main::getWindow()->isKeyDown(GLFW_KEY_S);
		bool keyD = Main::getWindow()->isKeyDown(GLFW_KEY_D);
		if (!keyW && !keyA && !keyS && !keyD)
			return (false);
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
		if (!keyW && !keyA && !keyS && !keyD)
			return (false);
		double angle = rotY;
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
		float addX = MOVEMENT_SPEED * cos(angle / 180. * M_PI);
		float addZ = MOVEMENT_SPEED * sin(angle / 180. * M_PI);
		//if (checkCollisionX(addX))
		//	addX = 0;
		//if (checkCollisionZ(addZ))
		//	addZ = 0;
		this->posX += addX;
		this->posZ += addZ;
		return (true);
	}

	bool Player::handleMovementY()
	{
		bool keyLShift = Main::getWindow()->isKeyDown(GLFW_KEY_LEFT_SHIFT);
		bool keySpace = Main::getWindow()->isKeyDown(GLFW_KEY_SPACE);
		if (keyLShift == keySpace)
			return (false);
		float addY = 0;
		if (keyLShift)
			addY = -MOVEMENT_SPEED;
		else if (keySpace)
			addY = MOVEMENT_SPEED;
		//if (checkCollisionY(addY))
		//	return;
		this->posY += addY;
		return (true);
	}

	bool Player::handleMovement()
	{
		bool moveXZ = handleMovementXZ();
		bool moveY = handleMovementY();
		return (moveXZ || moveY);
	}

	bool Player::handleRotation()
	{
		int32_t mouseX = Main::getWindow()->getMouseX();
		int32_t mouseY = Main::getWindow()->getMouseY();
		if (mouseX == this->oldMouseX && mouseY == this->oldMouseY)
			return (false);
		this->rotY += (mouseX - this->oldMouseX) / 20.;
		this->rotX += (mouseY - this->oldMouseY) / 20.;
		this->oldMouseX = mouseX;
		this->oldMouseY = mouseY;
		this->rotY = std::fmod(this->rotY, 360);
		if (this->rotY < 0)
			this->rotY += 360;
		if (this->rotX > 90)
			this->rotX = 90;
		else if (this->rotX < -90)
			this->rotX = -90;
		return (true);
	}

	void Player::tick()
	{
		bool move = handleMovement();
		bool rot = handleRotation();
		if (!move && !rot)
			return;
		this->viewMat = glm::mat4(1.);
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rotX / 180. * M_PI, 0).x, glm::vec3(1, 0, 0));
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rotY / 180. * M_PI, 0).x, glm::vec3(0, 1, 0));
		this->viewMat = glm::translate(this->viewMat, glm::vec3(-this->posX, -this->posY, -this->posZ));
	}

}
