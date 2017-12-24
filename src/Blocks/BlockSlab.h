#ifndef BLOCK_SLAB_H
# define BLOCK_SLAB_H

# include "./BlockBase.h"

namespace voxel
{

	class BlockSlab : public BlockBase
	{

	public:
		BlockSlab(uint8_t id);
		virtual void draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights);

	};

}

#endif
