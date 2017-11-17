#ifndef BLOCK_LIQUID_H
# define BLOCK_LIQUID_H

# include "./Block.h"

namespace voxel
{

	class BlockLiquid : public Block
	{

	private:
		uint8_t level;

	public:
		BlockLiquid(uint8_t id);
		virtual void draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tesselator, uint8_t visibleFaces, float *lights);

	};

}

#endif
