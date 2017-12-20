#ifndef BLOCK_LIQUID_H
# define BLOCK_LIQUID_H

# include "./Block.h"

namespace voxel
{

	class BlockLiquid : public Block
	{

	private:
		float texX;
		float texY;

	public:
		BlockLiquid(uint8_t id);
		virtual void draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights);
		BlockLiquid *setTex(float texX, float texY);
		BlockLiquid *setTexX(float texX);
		BlockLiquid *setTexY(float texY);

	};

}

#endif
