#ifndef PLAYER_RAYCAST_H
# define PLAYER_RAYCAST_H

# include "AABB.h"
# include <librender/Shader/VertexBuffer.h>
# include <glm/vec3.hpp>

using librender::VertexBuffer;

namespace voxel
{

	class ChunkBlock;
	class Player;
	class Chunk;

	class PlayerRaycast
	{

	private:
		Player &player;
		VertexBuffer breakTexCoordsBuffer;
		VertexBuffer hoverVertexesBuffer;
		VertexBuffer breakVertexesBuffer;
		VertexBuffer breakIndicesBuffer;
		glm::vec3 pos;
		uint32_t todoTicks;
		uint32_t doneTicks;
		uint8_t face;
		bool found;
		void onRightClick(Chunk *chunk, glm::vec3 &pos);
		void onLeftClick(Chunk *chunk, ChunkBlock *block, glm::vec3 &relative, glm::vec3 &pos);
		void buildBreakTexCoords();
		void buildHoverVertexes(AABB aabb);

	public:
		PlayerRaycast(Player &player);
		~PlayerRaycast();
		void tick();
		void raycast();
		void draw();
		inline bool isFound() {return (this->found);};
		inline glm::vec3 &getPos() {return (this->pos);};

	};

}

#endif
