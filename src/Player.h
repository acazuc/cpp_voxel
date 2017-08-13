#ifndef PLAYER_H
# define PLAYER_H

# include <glm/glm.hpp>
# include <cstdint>

namespace voxel
{

	class Player
	{

	private:
		int32_t oldMouseX;
		int32_t oldMouseY;
		glm::mat4 viewMat;
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

	public:
		Player();
		void tick();
		inline glm::mat4 &getViewMat() {return (this->viewMat);};

	};

}

#endif
