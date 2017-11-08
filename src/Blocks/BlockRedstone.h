#ifndef BLOCK_REDSTONE_H
# define BLOCK_REDSTONE_H

# include "./Block.h"

namespace voxel
{

	class BlockRedstone : public Block
	{

	private:
		float texX;
		float texY;

	public:
		BlockRedstone(uint8_t id);
		virtual void draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tesselator, uint8_t visibleFaces, float *lights);
		BlockRedstone *setTex(float texX, float texY);
		BlockRedstone *setTexX(float texX);
		BlockRedstone *setTexY(float texY);

	};

}

#endif
