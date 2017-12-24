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
		VertexBuffer texCoordsBuffer;
		VertexBuffer vertexesBuffer;
		VertexBuffer indicesBuffer;
		VertexBuffer colorsBuffer;
		uint64_t vertexNumber;
		int64_t created;
		uint8_t number;
		uint8_t type;

	public:
		DroppedBlock(World &world, Chunk *chunk, uint8_t type, Vec3 pos, Vec3 vel);
		void draw();

	};

}

#endif
