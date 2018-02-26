#include "ChunkStorage.h"
#include "NBT/NBTCompoundSanitizer.h"
#include "NBT/NBTMgr.h"
#include "Chunk.h"
#include <cstring>

namespace voxel
{

	ChunkStorage::ChunkStorage(uint8_t id)
	: id(id)
	{
		//Empty
	}

	ChunkStorage::~ChunkStorage()
	{
		//Empty
	}

	void ChunkStorage::initNBT(NBTTagCompound *NBT)
	{
		std::memset(&this->NBT, 0, sizeof(this->NBT));
		this->NBT.NBT = NBT;
		if (!this->NBT.NBT)
			this->NBT.NBT = new NBTTagCompound("");
		NBTCompoundSanitizer sanitizer(this->NBT.NBT);
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("Y", &this->NBT.Y, this->id));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByteArray("Blocks", &this->NBT.Blocks, CHUNK_WIDTH * CHUNK_WIDTH * 16));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByteArray("Add", &this->NBT.Add, CHUNK_WIDTH * CHUNK_WIDTH * 8));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByteArray("Data", &this->NBT.Data, CHUNK_WIDTH * CHUNK_WIDTH * 8));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByteArray("BlockLight", &this->NBT.BlockLight, CHUNK_WIDTH * CHUNK_WIDTH * 8));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByteArray("SkyLight", &this->NBT.SkyLight, CHUNK_WIDTH * CHUNK_WIDTH * 8));
		sanitizer.sanitize();
		if (this->NBT.Blocks->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 16)
		{
			this->NBT.Blocks->getValues().clear();
			this->NBT.Blocks->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 16, 0);
		}
		if (this->NBT.Add->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 8)
		{
			this->NBT.Add->getValues().clear();
			this->NBT.Add->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
		}
		if (this->NBT.Data->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 8)
		{
			this->NBT.Data->getValues().clear();
			this->NBT.Data->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
		}
		if (this->NBT.BlockLight->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 8)
		{
			this->NBT.BlockLight->getValues().clear();
			this->NBT.BlockLight->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
		}
		if (this->NBT.SkyLight->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 8)
		{
			this->NBT.SkyLight->getValues().clear();
			this->NBT.SkyLight->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
		}
	}

	void ChunkStorage::fillBuffers(Chunk *chunk, ChunkTessellator &tessellator, uint8_t layer)
	{
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t y = 0; y < 16; ++y)
			{
				for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
				{
					Vec3 pos(chunk->getX() + x, this->id * 16 + y, chunk->getZ() + z);
					ChunkBlock block;
					block.setType(this->NBT.Blocks->getValues()[getXYZId(x, y, z)]);
					block.fillBuffers(chunk, pos, tessellator, layer);
				}
			}
		}
	}

	void ChunkStorage::resetLights()
	{
		std::memset(this->NBT.BlockLight->getValues().data(), 0, this->NBT.BlockLight->getValues().size());
		std::memset(this->NBT.SkyLight->getValues().data(), 0, this->NBT.SkyLight->getValues().size());
	}

	void ChunkStorage::setBlock(int32_t x, int32_t y, int32_t z, uint8_t type)
	{
		this->NBT.Blocks->getValues()[getXYZId(x, y, z)] = type;
	}

	ChunkBlock *ChunkStorage::getBlock(int32_t x, int32_t y, int32_t z)
	{
		return (reinterpret_cast<ChunkBlock*>(&this->NBT.Blocks->getValues()[getXYZId(x, y, z)]));
	}

	uint8_t ChunkStorage::getLight(int32_t x, int32_t y, int32_t z)
	{
		return (std::max(getSkyLight(x, y, z), getBlockLight(x, y, z)));
	}

	void ChunkStorage::setSkyLight(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		uint32_t idx = getXYZId(x, y, z);
		if (idx & 1)
			this->NBT.SkyLight->getValues()[idx / 2] = (this->NBT.SkyLight->getValues()[idx / 2] & 0xf0) | (light & 0xf);
		else
			this->NBT.SkyLight->getValues()[idx / 2] = (this->NBT.SkyLight->getValues()[idx / 2] & 0x0f) | ((light & 0xf) << 4);
	}

	uint8_t ChunkStorage::getSkyLight(int32_t x, int32_t y, int32_t z)
	{
		uint32_t idx = getXYZId(x, y, z);
		if (idx & 1)
			return (this->NBT.SkyLight->getValues()[idx / 2] & 0xf);
		return ((this->NBT.SkyLight->getValues()[idx / 2] >> 4) & 0xf);
	}

	void ChunkStorage::setBlockLight(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		uint32_t idx = getXYZId(x, y, z);
		if (idx & 1)
			this->NBT.BlockLight->getValues()[idx / 2] = (this->NBT.BlockLight->getValues()[idx / 2] & 0xf0) | (light & 0xf);
		else
			this->NBT.BlockLight->getValues()[idx / 2] = (this->NBT.BlockLight->getValues()[idx / 2] & 0x0f) | ((light & 0xf) << 4);
	}

	uint8_t ChunkStorage::getBlockLight(int32_t x, int32_t y, int32_t z)
	{
		uint32_t idx = getXYZId(x, y, z);
		if (idx & 1)
			return (this->NBT.BlockLight->getValues()[idx / 2] & 0xf);
		return ((this->NBT.BlockLight->getValues()[idx / 2] >> 4) & 0xf);
	}

	uint32_t ChunkStorage::getXYZId(int8_t x, int8_t y, int8_t z)
	{
		return ((y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x);
	}

}
