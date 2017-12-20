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
		void setBlock(int32_t x, int32_t y, int32_t z, uint8_t type);
		ChunkBlock *getBlock(int32_t x, int32_t y, int32_t z);
		uint8_t getLight(int32_t x, int32_t y, int32_t z);
		void setSkyLight(int32_t x, int32_t y, int32_t z, uint8_t light);
		uint8_t getSkyLight(int32_t x, int32_t y, int32_t z);
		void setBlockLight(int32_t x, int32_t y, int32_t z, uint8_t light);
		uint8_t getBlockLight(int32_t x, int32_t y, int32_t z);
		uint32_t getXYZId(int8_t x, int8_t y, int8_t z);

	};

}

#endif
