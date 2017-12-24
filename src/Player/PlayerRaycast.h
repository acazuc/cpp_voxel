#ifndef PLAYER_RAYCAST_H
# define PLAYER_RAYCAST_H

# include "AABB.h"
# include <librender/Shader/VertexBuffer.h>

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
		Vec3 pos;
		uint32_t todoTicks;
		uint32_t doneTicks;
		uint8_t face;
		bool found;
		void onRightClick(Chunk *chunk, Vec3 &pos);
		void onLeftClick(Chunk *chunk, ChunkBlock *block, Vec3 &relative, Vec3 &pos);
		void buildBreakTexCoords();
		void buildHoverVertexes(AABB aabb);

	public:
		PlayerRaycast(Player &player);
		~PlayerRaycast();
		void tick();
		void raycast();
		void draw();
		inline bool isFound() {return (this->found);};
		inline Vec3 &getPos() {return (this->pos);};

	};

}

#endif
