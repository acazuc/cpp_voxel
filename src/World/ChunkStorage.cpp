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
		std::memset(this->blockLights, 0, sizeof(this->blockLights));
		std::memset(this->skyLights, 0, sizeof(this->skyLights));
	}

	ChunkStorage::~ChunkStorage()
	{
		//Empty
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
					ChunkBlock block;
					block.setType(this->blocks[getXYZId(x, y, z)]);
					block.fillBuffers(chunk, pos, tessellator, layer);
				}
			}
		}
	}

	void ChunkStorage::resetLights()
	{
		std::memset(this->skyLights, 0, sizeof(this->skyLights));
		std::memset(this->blockLights, 0, sizeof(this->blockLights));
	}

	void ChunkStorage::setBlock(glm::vec3 pos, uint8_t type)
	{
		this->blocks[getXYZId(pos)] = type;
	}

	ChunkBlock *ChunkStorage::getBlock(glm::vec3 pos)
	{
		return (reinterpret_cast<ChunkBlock*>(&this->blocks[getXYZId(pos)]));
	}

	uint8_t ChunkStorage::getLight(glm::vec3 pos)
	{
		return (std::max(getSkyLight(pos), getBlockLight(pos)));
	}

	void ChunkStorage::setSkyLight(glm::vec3 pos, uint8_t light)
	{
		uint32_t idx = getXYZId(pos);
		if (idx & 1)
			this->skyLights[idx / 2] = (this->skyLights[idx / 2] & 0xf0) | (light & 0xf);
		else
			this->skyLights[idx / 2] = (this->skyLights[idx / 2] & 0x0f) | ((light & 0xf) << 4);
	}

	uint8_t ChunkStorage::getSkyLight(glm::vec3 pos)
	{
		uint32_t idx = getXYZId(pos);
		if (idx & 1)
			return (this->skyLights[idx / 2] & 0xf);
		return ((this->skyLights[idx / 2] >> 4) & 0xf);
	}

	void ChunkStorage::setBlockLight(glm::vec3 pos, uint8_t light)
	{
		uint32_t idx = getXYZId(pos);
		if (idx & 1)
			this->blockLights[idx / 2] = (this->blockLights[idx / 2] & 0xf0) | (light & 0xf);
		else
			this->blockLights[idx / 2] = (this->blockLights[idx / 2] & 0x0f) | ((light & 0xf) << 4);
	}

	uint8_t ChunkStorage::getBlockLight(glm::vec3 pos)
	{
		uint32_t idx = getXYZId(pos);
		if (idx & 1)
			return (this->blockLights[idx / 2] & 0xf);
		return ((this->blockLights[idx / 2] >> 4) & 0xf);
	}

	uint32_t ChunkStorage::getXYZId(glm::vec3 pos)
	{
		return (getXYZId((int8_t)pos.x, (int8_t)pos.y, (int8_t)pos.z));
	}

	uint32_t ChunkStorage::getXYZId(int8_t x, int8_t y, int8_t z)
	{
		return ((y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x);
	}

}
