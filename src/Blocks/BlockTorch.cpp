#include "BlockTorch.h"

namespace voxel
{

	BlockTorch::BlockTorch(uint8_t id)
	: Block(id)
	{
		//Empty
	}

	void BlockTorch::draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)chunk;
		(void)pos;
		(void)tessellator;
		(void)visibleFaces;
		(void)lights;
	}

	BlockTorch *BlockTorch::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return (this);
	}

	BlockTorch *BlockTorch::setTexX(float texX)
	{
		this->texX = texX;
		return (this);
	}

	BlockTorch *BlockTorch::setTexY(float texY)
	{
		this->texY = texY;
		return (this);
	}

}
