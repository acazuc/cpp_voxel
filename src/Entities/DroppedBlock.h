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
		World &world;
		uint64_t vertexNumber;
		int64_t created;
		uint8_t number;
		uint8_t type;

	public:
		DroppedBlock(World &world, uint8_t type);
		void draw();

	};

}

#endif
