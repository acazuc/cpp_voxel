#ifndef PLAYER_H
# define PLAYER_H

# include "./Player/PlayerRaycast.h"
# include <glm/glm.hpp>
# include <cstdint>


namespace voxel
{

	class World;

	class Player
	{

	private:
		PlayerRaycast raycast;
		glm::mat4 projMat;
		glm::mat4 viewMat;
		World &world;
		int64_t fallStarted;
		int32_t oldMouseX;
		int32_t oldMouseY;
		float gravity;
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
		bool isOnFloor;
		bool hasJumped;
		bool flying;
		bool handleMovementXZ();
		bool handleMovementY();
		bool handleMovement();
		bool handleRotation();
		bool checkCollisionX(float addX);
		bool checkCollisionY(float addY);
		bool checkCollisionZ(float addZ);
		bool checkCollideBlock(float x, float y, float z);

	public:
		Player(World &world);
		void tick();
		void draw();
		inline glm::mat4 &getProjMat() {return (this->projMat);};
		inline glm::mat4 &getViewMat() {return (this->viewMat);};
		inline World &getWorld() {return (this->world);};
		inline float getPosX() {return (this->posX);};
		inline float getPosY() {return (this->posY);};
		inline float getPosZ() {return (this->posZ);};
		inline float getRotX() {return (this->rotX);};
		inline float getRotY() {return (this->rotY);};
		inline float getRotZ() {return (this->rotZ);};

	};

}

#endif
