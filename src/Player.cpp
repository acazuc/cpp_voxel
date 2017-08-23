#include "Player.h"
#include "Utils/System.h"
#include "World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define MOVEMENT_SPEED .10
#define JUMP_FORCE .25
#define GRAVITY 1.5

namespace voxel
{

	Player::Player(World &world)
	: world(world)
	, fallStarted(0)
	, oldMouseX(0)
	, oldMouseY(0)
	, gravity(0)
	, posX(0)
	, posY(128)
	, posZ(0)
	, rotX(0)
	, rotY(0)
	, rotZ(0)
	, isOnFloor(true)
	{
		this->fallStarted = System::nanotime();
		this->projMat = glm::perspective(glm::radians(80.), Main::getWindow()->getWidth() / static_cast<double>(Main::getWindow()->getHeight()), .1, 1000.);
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
		float addX = MOVEMENT_SPEED * std::cos(angle / 180. * M_PI);
		float addZ = MOVEMENT_SPEED * std::sin(angle / 180. * M_PI);
		if (checkCollisionX(addX))
			addX = 0;
		if (checkCollisionZ(addZ))
			addZ = 0;
		this->posX += addX;
		this->posZ += addZ;
		return (true);
	}

	bool Player::handleMovementY()
	{
		/*bool keyLShift = Main::getWindow()->isKeyDown(GLFW_KEY_LEFT_SHIFT);
		bool keySpace = Main::getWindow()->isKeyDown(GLFW_KEY_SPACE);
		if (keyLShift == keySpace)
			return (false);
		float addY = 0;
		if (keyLShift)
			addY = -MOVEMENT_SPEED;
		else if (keySpace)
			addY = MOVEMENT_SPEED;
		if (checkCollisionY(addY))
			addY = 0;
		this->posY += addY;*/
		if (this->isOnFloor && !checkCollisionY(-0.0001))
		{
			this->fallStarted = System::nanotime();
			this->isOnFloor = false;
			this->hasJumped = false;
		}
		if (this->isOnFloor)
		{
			if (Main::getWindow()->isKeyDown(GLFW_KEY_SPACE))
			{
				this->isOnFloor = false;
				this->fallStarted = System::nanotime();
				this->gravity = -JUMP_FORCE;
				this->hasJumped = true;
			}
		}
		else
		{
			this->gravity = GRAVITY * ((System::nanotime() - this->fallStarted) / 1000000000.);
			if (this->hasJumped)
				this->gravity -= JUMP_FORCE;
		}
		if (!this->gravity)
			return (false);
		float addY = -this->gravity;
		if (checkCollisionY(addY))
		{
			this->isOnFloor = true;
			addY = 0;
		}
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
		if (!move && !rot && false)
			return;
		this->viewMat = glm::mat4(1.);
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rotX / 180. * M_PI, 0).x, glm::vec3(1, 0, 0));
		this->viewMat = glm::rotate(this->viewMat, glm::vec2(this->rotY / 180. * M_PI, 0).x, glm::vec3(0, 1, 0));
		this->viewMat = glm::translate(this->viewMat, glm::vec3(-this->posX, -this->posY, -this->posZ));
		this->world.getFrustum().update();
	}

	bool Player::checkCollisionX(float addX)
	{
		if (addX > 0)
		{
			for (int32_t i = 0; i < addX; ++i)
			{
				if (checkCollideBlock(i + .2, .19f, 0) || checkCollideBlock(i + .2f, -1.69f, 0))
				{
					this->posX = round(this->posX + i + .2f) - .7f;
					return (true);
				}
			}
			if (checkCollideBlock(addX + .2, .19f, 0) || checkCollideBlock(addX + .2f, -1.69f, 0))
			{
				this->posX = round(this->posX + addX + .2f) - .7f;
				return (true);
			}
		}
		else if (addX < 0)
		{
			for (int32_t i = 0; i > addX; --i)
			{
				if (checkCollideBlock(i - .2f, .19f, 0) || checkCollideBlock(i - .2f, -1.69f, 0))
				{
					this->posX = round(this->posX + i - .2f) + .7f;
					return (true);
				}
			}
			if (checkCollideBlock(addX - .2f, .19f, 0) || checkCollideBlock(addX - .2f, -1.69f, 0))
			{
				this->posX = round(this->posX + addX - .2f) + .7f;
				return (true);
			}
		}
		return (false);
	}

	bool Player::checkCollisionY(float addY)
	{
		if (addY > 0)
		{
			for (int32_t i = 0; i < addY; ++i)
			{
				if (checkCollideBlock(0, i + .2f, 0))
				{
					this->posY = round(this->posY + i + .2f) - .7f;
					return (true);
				}
			}
			if (checkCollideBlock(0, addY + .2f, 0))
			{
				this->posY = round(this->posY + addY + .2f) - .7f;
				return (true);
			}
		}
		else if (addY < 0)
		{
			for (int32_t i = 0; i > addY; --i)
			{
				if (checkCollideBlock(0, i - 1.7f, 0))
				{
					this->posY = round(this->posY + i - 1.7f) + 2.2f;
					return (true);
				}
			}
			if (checkCollideBlock(0, addY - 1.7f, 0))
			{
				this->posY = round(this->posY + addY - 1.7f) + 2.2f;
				return (true);
			}
		}
		return (false);
	}

	bool Player::checkCollisionZ(float addZ)
	{
		if (addZ > 0)
		{
			for (int32_t i = 0; i < addZ; ++i)
			{
				if (checkCollideBlock(0, .19f, i + .2f) || checkCollideBlock(0, -1.69f, i + .2f))
				{
					this->posZ = round(this->posZ + i + .2f) - .7f;
					return (true);
				}
			}
			if (checkCollideBlock(0, .19f, addZ + .2f) || checkCollideBlock(0, -1.69f, addZ + .2f))
			{
				this->posZ = round(this->posZ + addZ + .2f) - .7f;
				return (true);
			}
		}
		else if (addZ < 0)
		{
			for (int32_t i = 0; i > addZ; --i)
			{
				if (checkCollideBlock(0, .19f, i - .2f) || checkCollideBlock(0, -1.69f, i - .2f))
				{
					this->posZ = round(this->posZ + i - .2f) + .7f;
					return (true);
				}
			}
			if (checkCollideBlock(0, .19f, addZ - .2f) || checkCollideBlock(0, -1.69f, addZ - .2f))
			{
				this->posZ = round(this->posZ + addZ - .2f) + .7f;
				return (true);
			}
		}
		return (false);
	}

	bool Player::checkCollideBlock(float x, float y, float z)
	{
		y = round(y + this->posY);
		if (y >= 0 && y < CHUNK_HEIGHT)
		{
			x = round(x + this->posX);
			z = round(z + this->posZ);
			int32_t chunkX = floor(x / CHUNK_WIDTH) * CHUNK_WIDTH;
			int32_t chunkZ = floor(z / CHUNK_WIDTH) * CHUNK_WIDTH;
			Chunk *chunk = this->world.getChunk(chunkX, chunkZ);
			if (!chunk)
				return (false);
			Block *block = chunk->getBlockAt(x - chunkX, y, z - chunkZ);
			if (!block)
				return (false);
			return (true);
		}
		return (false);
	}

}
