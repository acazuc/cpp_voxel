#ifndef BLOCK_GRASS_H
# define BLOCK_GRASS_H

# include "./BlockBase.h"

namespace voxel
{

	class BlockGrass : public BlockBase
	{

	private:

	public:
		BlockGrass(uint8_t id);
		void draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights);

	};

}

#endif
