#ifndef CHUNK_STORAGE_H
# define CHUNK_STORAGE_H

# include "ChunkBlock.h"

namespace voxel
{

	class Chunk;

	class ChunkStorage
	{

	private:
		uint8_t blocks[16 * 16 * 16];
		uint8_t blockLights[16 * 16 * 8];
		uint8_t skyLights[16 * 16 * 8];
		int32_t y;

	public:
		ChunkStorage(int32_t y);
		~ChunkStorage();
		void fillBuffers(Chunk *chunk, ChunkTessellator &tessellator, uint8_t layer);
		void resetLights();
		void setBlock(glm::vec3 pos, uint8_t type);
		ChunkBlock *getBlock(glm::vec3 pos);
		uint8_t getLight(glm::vec3 pos);
		void setSkyLight(glm::vec3 pos, uint8_t light);
		uint8_t getSkyLight(glm::vec3 pos);
		void setBlockLight(glm::vec3 pos, uint8_t light);
		uint8_t getBlockLight(glm::vec3 pos);
		uint32_t getXYZId(glm::vec3 pos);
		uint32_t getXYZId(int8_t x, int8_t y, int8_t z);

	};

}

#endif
