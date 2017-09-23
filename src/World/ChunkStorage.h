#ifndef CHUNK_STORAGE_H
# define CHUNK_STORAGE_H

# include "ChunkBlock.h"

namespace voxel
{

	class Chunk;

	class ChunkStorage
	{

	private:
		ChunkBlock *blocks;
		int32_t y;

	public:
		ChunkStorage(int32_t y);
		~ChunkStorage();
		void fillBuffers(Chunk *chunk, ChunkTessellator &tessellator, uint8_t layer);
		void resetLights();
		void setBlock(glm::vec3 pos, uint8_t type);
		ChunkBlock *getBlock(glm::vec3 pos);
		uint32_t getXYZId(glm::vec3 pos);
		uint32_t getXYZId(int8_t x, int8_t y, int8_t z);

	};

}

#endif
