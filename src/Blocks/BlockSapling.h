#ifndef BLOCK_SAPLING_H
# define BLOCK_SAPLING_H

# include "./Block.h"

namespace voxel
{

	class BlockSapling : public Block
	{

	private:
		float texX;
		float texY;

	public:
		BlockSapling(uint8_t id);
		virtual void draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights);
		virtual float getDestroyTexX();
		virtual float getDestroyTexY();
		BlockSapling *setTex(float texX, float texY);
		BlockSapling *setTexX(float texX);
		BlockSapling *setTexY(float texY);

	};

}

#endif
