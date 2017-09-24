#include "ChunkStorage.h"
#include "Debug.h"
#include "Chunk.h"
#include <cstring>

namespace voxel
{

	ChunkStorage::ChunkStorage(int32_t y)
	: y(y)
	{
		std::memset(this->blocks, 0, sizeof(this->blocks));
	}

	void ChunkStorage::fillBuffers(Chunk *chunk, ChunkTessellator &tessellator, uint8_t layer)
	{
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t y = 0; y < 16; ++y)
			{
				for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
				{
					glm::vec3 pos(chunk->getX() + x, this->y + y, chunk->getZ() + z);
					this->blocks[getXYZId(x, y, z)].fillBuffers(chunk, pos, tessellator, layer);
					(void)chunk;
					(void)tessellator;
					(void)layer;
				}
			}
		}
	}

	void ChunkStorage::resetLights()
	{
		for (uint32_t i = 0; i < CHUNK_WIDTH * 16 * CHUNK_WIDTH; ++i)
			this->blocks[i].setLight(0);
	}

	void ChunkStorage::setBlock(glm::vec3 pos, uint8_t type)
	{
		this->blocks[getXYZId(pos)].setType(type);
	}

	ChunkBlock *ChunkStorage::getBlock(glm::vec3 pos)
	{
		return (&this->blocks[getXYZId(pos)]);
	}

	uint32_t ChunkStorage::getXYZId(glm::vec3 pos)
	{
		return (getXYZId((int8_t)pos.x, (int8_t)pos.y, (int8_t)pos.z));
	}

	uint32_t ChunkStorage::getXYZId(int8_t x, int8_t y, int8_t z)
	{
		return ((x * 16 + y) * CHUNK_WIDTH + z);
	}

}
