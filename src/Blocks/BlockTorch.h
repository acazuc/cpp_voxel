#ifndef BLOCK_TORCH_H
# define BLOCK_TORCH_H

# include "./Block.h"

namespace voxel
{

	class BlockTorch : public Block
	{

	private:
		float texX;
		float texY;

	public:
		BlockTorch(uint8_t id);
		virtual void draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights);
		BlockTorch *setTex(float texX, float texY);
		BlockTorch *setTexX(float texX);
		BlockTorch *setTexY(float texY);

	};

}

#endif
