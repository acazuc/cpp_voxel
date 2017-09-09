#ifndef PLAYER_RAYCAST_H
# define PLAYER_RAYCAST_H

# include <librender/Shader/VertexBuffer.h>
# include <glm/vec3.hpp>

using librender::VertexBuffer;

namespace voxel
{

	class Player;

	class PlayerRaycast
	{

	private:
		Player &player;
		VertexBuffer vertexesBuffer;
		VertexBuffer colorsBuffer;
		glm::vec3 pos;
		uint8_t face;
		bool found;
		void raycast();

	public:
		PlayerRaycast(Player &player);
		~PlayerRaycast();
		void tick();
		void draw();

	};

}

#endif
