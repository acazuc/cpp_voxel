#include "ChunkStorage.h"
#include "Debug.h"
#include "Chunk.h"
#include <cstring>

namespace voxel
{

	ChunkStorage::ChunkStorage(uint8_t id)
	: id(id)
	{
		/*std::memset(this->blocks, 0, sizeof(this->blocks));
		std::memset(this->blockLights, 0, sizeof(this->blockLights));
		std::memset(this->skyLights, 0, sizeof(this->skyLights));*/
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
		for (std::vector<NBTTag*>::iterator iter = this->NBT.NBT->getTags().begin(); iter != this->NBT.NBT->getTags().end(); ++iter)
		{
			if (!(*iter)->getName().compare("Y") && (*iter)->getType() == NBT_TAG_BYTE)
				this->NBT.Y = reinterpret_cast<NBTTagByte*>(*iter);
			else if (!(*iter)->getName().compare("Blocks") && (*iter)->getType() == NBT_TAG_BYTE_ARRAY)
				this->NBT.Blocks = reinterpret_cast<NBTTagByteArray*>(*iter);
			else if (!(*iter)->getName().compare("Add") && (*iter)->getType() == NBT_TAG_BYTE_ARRAY)
				this->NBT.Add = reinterpret_cast<NBTTagByteArray*>(*iter);
			else if (!(*iter)->getName().compare("Data") && (*iter)->getType() == NBT_TAG_BYTE_ARRAY)
				this->NBT.Data = reinterpret_cast<NBTTagByteArray*>(*iter);
			else if (!(*iter)->getName().compare("BlockLight") && (*iter)->getType() == NBT_TAG_BYTE_ARRAY)
				this->NBT.BlockLight = reinterpret_cast<NBTTagByteArray*>(*iter);
			else if (!(*iter)->getName().compare("SkyLight") && (*iter)->getType() == NBT_TAG_BYTE_ARRAY)
				this->NBT.SkyLight = reinterpret_cast<NBTTagByteArray*>(*iter);
			else
				goto erase;
			continue;
erase:
			this->NBT.NBT->getTags().erase(iter);
			iter = this->NBT.NBT->getTags().begin();
			if (iter == this->NBT.NBT->getTags().end())
				break;
		}
		if (!this->NBT.Y)
		{
			this->NBT.Y = new NBTTagByte("Y");
			this->NBT.Y->setValue(this->id);
			this->NBT.NBT->addTag(this->NBT.Y);
		}
		if (!this->NBT.Blocks)
		{
			this->NBT.Blocks = new NBTTagByteArray("Blocks");
			this->NBT.Blocks->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 16, 0);
			this->NBT.NBT->addTag(this->NBT.Blocks);
		}
		if (this->NBT.Blocks->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 16)
		{
			this->NBT.Blocks->getValues().clear();
			this->NBT.Blocks->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 16, 0);
		}
		if (!this->NBT.Add)
		{
			this->NBT.Add = new NBTTagByteArray("Add");
			this->NBT.Add->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
			this->NBT.NBT->addTag(this->NBT.Add);
		}
		if (this->NBT.Add->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 8)
		{
			this->NBT.Add->getValues().clear();
			this->NBT.Add->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
		}
		if (!this->NBT.Data)
		{
			this->NBT.Data = new NBTTagByteArray("Data");
			this->NBT.Data->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
			this->NBT.NBT->addTag(this->NBT.Data);
		}
		if (this->NBT.Data->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 8)
		{
			this->NBT.Data->getValues().clear();
			this->NBT.Data->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
		}
		if (!this->NBT.BlockLight)
		{
			this->NBT.BlockLight = new NBTTagByteArray("BlockLight");
			this->NBT.BlockLight->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
			this->NBT.NBT->addTag(this->NBT.BlockLight);
		}
		if (this->NBT.BlockLight->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH * 8)
		{
			this->NBT.BlockLight->getValues().clear();
			this->NBT.BlockLight->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
		}
		if (!this->NBT.SkyLight)
		{
			this->NBT.SkyLight = new NBTTagByteArray("BlockLight");
			this->NBT.SkyLight->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH * 8, 0);
			this->NBT.NBT->addTag(this->NBT.SkyLight);
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
