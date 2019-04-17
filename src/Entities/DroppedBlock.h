#ifndef DROPPED_BLOCK_H
# define DROPPED_BLOCK_H

# include "Entity.h"
# include <librender/Shader/VertexBuffer.h>

using librender::VertexBuffer;

namespace voxel
{

	class DroppedBlock : public Entity
	{

	private:
		VertexBuffer positionBuffer;
		VertexBuffer indiceBuffer;
		VertexBuffer colorBuffer;
		VertexBuffer uvBuffer;
		uint64_t indicesNb;
		int64_t created;
		uint8_t number;
		uint8_t type;

	public:
		DroppedBlock(World &world, Chunk *chunk, uint8_t type, Vec3 pos, Vec3 vel);
		void draw();

	};

}

#endif
