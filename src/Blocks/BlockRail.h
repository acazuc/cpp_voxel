#ifndef BLOCK_RAIL_H
# define BLOCK_RAIL_H

# include "./Block.h"

namespace voxel
{

	class BlockRail : public Block
	{

	private:
		float texX;
		float texY;

	public:
		BlockRail(uint8_t id);
		virtual void draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights);
		virtual float getDestroyTexX();
		virtual float getDestroyTexY();
		BlockRail *setTex(float texX, float texY);
		BlockRail *setTexX(float texX);
		BlockRail *setTexY(float texY);

	};

}

#endif
