#ifndef BLOCK_LEAVES_H
# define BLOCK_LEAVES_H

# include "./BlockBase.h"

namespace voxel
{

	class BlockLeaves : public BlockBase
	{

	private:

	public:
		BlockLeaves(uint8_t id);
		void draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights);

	};

}

#endif
