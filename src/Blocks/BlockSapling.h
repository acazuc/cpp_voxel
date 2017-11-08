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
		virtual void draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tesselator, uint8_t visibleFaces, float *lights);
		BlockSapling *setTex(float texX, float texY);
		BlockSapling *setTexX(float texX);
		BlockSapling *setTexY(float texY);

	};

}

#endif
