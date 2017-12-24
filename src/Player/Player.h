#ifndef PLAYER_H
# define PLAYER_H

# include "Entities/Entity.h"
# include "PlayerInventory.h"
# include "PlayerRaycast.h"
# include <librender/Mat/Mat4.h>

using librender::Mat4;

namespace voxel
{

	class World;

	class Player : public Entity
	{

	private:
		PlayerInventory inventory;
		PlayerRaycast raycast;
		Mat4 viewProjMat;
		Mat4 projMat;
		Mat4 viewMat;
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
		inline PlayerInventory &getInventory() {return (this->inventory);};
		inline PlayerRaycast &getRaycast() {return (this->raycast);};
		inline Mat4 &getViewProjMat() {return (this->viewProjMat);};
		inline Mat4 &getProjMat() {return (this->projMat);};
		inline Mat4 &getViewMat() {return (this->viewMat);};
		inline uint8_t getEyeLight() {return (this->eyeLight);};
		inline bool isEyeInWater() {return (this->eyeInWater);};

	};

}

#endif
