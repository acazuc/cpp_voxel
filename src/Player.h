#ifndef PLAYER_H
# define PLAYER_H

# include <glm/glm.hpp>
# include <cstdint>

namespace voxel
{

	class World;

	class Player
	{

	private:
		glm::mat4 projMat;
		glm::mat4 viewMat;
		World &world;
		int32_t oldMouseX;
		int32_t oldMouseY;
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
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
		inline glm::mat4 &getProjMat() {return (this->projMat);};
		inline glm::mat4 &getViewMat() {return (this->viewMat);};
		inline float getPosX() {return (this->posX);};
		inline float getPosY() {return (this->posY);};
		inline float getPosZ() {return (this->posZ);};

	};

}

#endif
