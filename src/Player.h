#ifndef PLAYER_H
# define PLAYER_H

# include "./Player/PlayerRaycast.h"
# include "./Entity.h"
# include <glm/glm.hpp>


namespace voxel
{

	class Player : public Entity
	{

	private:
		PlayerRaycast raycast;
		glm::mat4 projMat;
		glm::mat4 viewMat;
		int32_t oldMouseX;
		int32_t oldMouseY;
		bool handleMovement();
		bool handleRotation();

	public:
		Player(World &world);
		void tick();
		void draw();
		inline glm::mat4 &getProjMat() {return (this->projMat);};
		inline glm::mat4 &getViewMat() {return (this->viewMat);};
		inline World &getWorld() {return (this->world);};

	};

}

#endif
