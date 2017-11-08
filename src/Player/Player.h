#ifndef PLAYER_H
# define PLAYER_H

# include "Entities/Entity.h"
# include "PlayerRaycast.h"
# include <glm/mat4x4.hpp>

namespace voxel
{

	class World;

	class Player : public Entity
	{

	private:
		PlayerRaycast raycast;
		glm::mat4 viewProjMat;
		glm::mat4 projMat;
		glm::mat4 viewMat;
		int32_t oldMouseX;
		int32_t oldMouseY;
		uint8_t eyeInWater;
		uint8_t eyeLight;
		void handleMovement();
		void handleRotation();

	public:
		Player(World &world);
		void update();
		void tick();
		void draw();
		inline PlayerRaycast &getRaycast() {return (this->raycast);};
		inline glm::mat4 &getViewProjMat() {return (this->viewProjMat);};
		inline glm::mat4 &getProjMat() {return (this->projMat);};
		inline glm::mat4 &getViewMat() {return (this->viewMat);};
		inline uint8_t getEyeLight() {return (this->eyeLight);};
		inline bool isEyeInWater() {return (this->eyeInWater);};

	};

}

#endif
