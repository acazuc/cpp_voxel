#include "Region.h"
#include "NBT/NBTBuffer.h"
#include "World.h"
#include "Debug.h"
#include <cstring>

namespace voxel
{

	Region::Region(World &world, int32_t x, int32_t z)
	: world(world)
	, x(x)
	, z(z)
	, file(NULL)
	{
		this->filename = "world/r." + std::to_string(this->x / (REGION_WIDTH * CHUNK_WIDTH)) + "." + std::to_string(this->z / (REGION_WIDTH * CHUNK_WIDTH)) + ".mca";
		std::memset(this->chunks, 0, sizeof(this->chunks));
		std::memset(this->storageTimestamp, 0, sizeof(this->storageTimestamp));
		std::memset(this->storageHeader, 0, sizeof(this->storageHeader));
		load();
	}

	Region::~Region()
	{
		save();
		if (this->file)
			std::fclose(this->file);
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
			delete (this->chunks[i]);
	}

	void Region::load()
	{
		if (!(this->file = std::fopen(this->filename.c_str(), "rb+")))
		{
			//No way to open O_CREAT | O_RDWR, so we're going full retard.
			LOG("Region file \"" << this->filename << " doesn't exists, creating it");
			if (!(this->file = std::fopen(this->filename.c_str(), "w")))
				ERROR("Failed to create region file \"" << this->filename << "\"");
			std::fclose(this->file);
			if (!(this->file = std::fopen(this->filename.c_str(), "rb+")))
				ERROR("Failed to create region file \"" << this->filename << "\"");
		}
		if (std::fseek(this->file, 0, SEEK_SET))
			ERROR("Failed to seek 0 on file \"" << this->filename << "\"");
		if (std::fread(this->storageHeader, sizeof(this->storageHeader), 1, this->file) != 1)
		{
			WARN("Failed to read header of \"" << this->filename << "\", writing empty one");
			if (std::fwrite(this->storageHeader, sizeof(this->storageHeader), 1, this->file) != 1)
				ERROR("Failed to write empty storage header of \"" << this->filename << "\"");
		}
		if (std::fread(this->storageTimestamp, sizeof(this->storageTimestamp), 1, this->file) != 1)
		{
			WARN("Failed to read timestamp of \"" << this->filename << "\", writing empty one");
			if (std::fwrite(this->storageTimestamp, sizeof(this->storageTimestamp), 1, this->file) != 1)
				ERROR("Failed to write empty storage timestamp of \"" << this->filename << "\"");
		}
		if (std::fseek(this->file, 0, SEEK_END))
			ERROR("Failed to get size of file \"" << this->filename << "\"");
		int64_t filesize = std::ftell(this->file);
		if (filesize == -1)
			ERROR("Failed to ftell \"" << this->filename << "\"");
		if (filesize & 0xfff)
		{
			char c = 0;
			for (uint32_t i = 0; i < (filesize & 0xfff); ++i)
			{
				if (std::fwrite(&c, 1, 1, this->file) != 1)
					ERROR("Failed to grow region file");
				++filesize;
			}
		}
		this->sectors.resize(filesize / REGION_SECTOR_SIZE, false);
		this->sectors[0] = true;
		this->sectors[1] = true;
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			uint32_t offsetAlloc = this->storageHeader[i];
			if (!offsetAlloc)
				continue;
			uint32_t offset = (offsetAlloc >> 8) & 0xffffff;
			uint32_t allocated = offsetAlloc & 0xff;
			if (offset + allocated >= this->sectors.size())
			{
				this->storageHeader[i] = 0;
				continue;
			}
			for (uint32_t j = 0; j < allocated; ++j)
				this->sectors[offset + j] = true;
		}
	}

	void Region::save()
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			if (!this->chunks[i])
				continue;
			saveChunk(this->chunks[i]);
		}
	}

	void Region::saveChunk(Chunk *chunk)
	{
		if (!chunk->isChanged())
			return;
		chunk->setChanged(false);
		uint32_t dataLen = chunk->getNBT()->getHeaderSize() + chunk->getNBT()->getDataSize() + 5;
		uint32_t sectorsLen = std::ceil(dataLen / (float)REGION_SECTOR_SIZE);
		char *data = new char[sectorsLen * REGION_SECTOR_SIZE];
		std::memset(data, 0, sectorsLen * REGION_SECTOR_SIZE);
		NBTBuffer buffer;
		buffer.data = data;
		buffer.pos = 0;
		buffer.len = sectorsLen * REGION_SECTOR_SIZE;
		buffer.writeInt32(dataLen - 4);
		buffer.writeInt8(2);
		chunk->getNBT()->writeHeader(&buffer);
		chunk->getNBT()->writeData(&buffer);
		if (!sectorsLen)
		{
			delete[] (data);
			return;
		}
		uint32_t headerPos = getXZId((chunk->getX() - this->x) / CHUNK_WIDTH, (chunk->getZ() - this->z) / CHUNK_WIDTH);
		uint32_t &offsetAlloc = this->storageHeader[headerPos];
		uint32_t offset = (offsetAlloc >> 8) & 0xffffff;
		uint8_t allocated = offsetAlloc & 0xff;
		if (!offset || allocated < sectorsLen)
		{
			if (offset)
			{
				for (uint32_t a = 0; a < allocated; ++a)
					this->sectors[a + offset] = false;
			}
			if (sectorsLen > this->sectors.size())
				goto newSector;
			for (uint32_t a = 0; a < this->sectors.size() - sectorsLen; ++a)
			{
				for (uint32_t b = 0; b < sectorsLen; ++b)
				{
					if (this->sectors[a + b])
						goto nextTest;
				}
				offset = a;
				allocated = sectorsLen;
				offsetAlloc = ((offset & 0xffffff) << 8) | allocated;
				for (uint32_t i = 0; i < allocated; ++i)
					this->sectors[offset + i] = true;
				goto write;
nextTest:
				continue;
			}
newSector:
			offset = this->sectors.size();
			allocated = sectorsLen;
			offsetAlloc = ((offset & 0xffffff) << 8) | allocated;
			this->sectors.resize(this->sectors.size() + allocated, true);
		}
write:
		if (std::fseek(this->file, offset * REGION_SECTOR_SIZE, SEEK_SET))
			ERROR("Failed to seek region \"" << this->filename << "\" chunk storage");
		if (std::fwrite(data, sectorsLen * REGION_SECTOR_SIZE, 1, this->file) != 1)
			ERROR("Failed to write region \"" << this->filename << "\" chunk storage");
		if (std::fseek(this->file, headerPos * 4, SEEK_SET))
			ERROR("Failed to seek region \"" << this->filename << "\" offset");
		if (std::fwrite(&this->storageHeader[headerPos], 4, 1, this->file) != 1)
			ERROR("Failed to write region \"" << this->filename << "\" offset");
		delete[] (data);
	}

	void Region::tick()
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			if (this->chunks[i])
				this->chunks[i]->tick();
		}
	}

	void Region::drawEntities()
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			if (this->chunks[i])
				this->chunks[i]->drawEntities();
		}
	}

	void Region::draw(uint8_t layer)
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			if (this->chunks[i])
				this->chunks[i]->draw(layer);
		}
	}

	void Region::moveGLBuffersToWorld()
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			if (this->chunks[i])
				this->chunks[i]->moveGLBuffersToWorld();
		}
	}

	void Region::generateChunk(int32_t x, int32_t z)
	{
		Chunk *chunk = getChunk(x, z);
		if (!chunk)
			chunk = createChunk(x, z);
		chunk->generate();
	}

	Chunk *Region::createChunk(int32_t x, int32_t z)
	{
		NBTTag *NBT = NULL;
		if (this->storageHeader[getXZId(x, z)])
		{
			LOG(this->filename);
			if (std::fseek(this->file, (this->storageHeader[getXZId(x, z)] >> 8) * REGION_SECTOR_SIZE, SEEK_SET))
				ERROR("Failed to seek section");
			LOG((this->storageHeader[getXZId(x, z)] >> 8) * REGION_SECTOR_SIZE);
			LOG(ftell(this->file));
			int32_t len;
			if (std::fread(&len, 4, 1, this->file) != 1)
				ERROR("Failed to read section length");
			len--;
			int8_t compression;
			if (std::fread(&compression, 1, 1, this->file) != 1)
				ERROR("Failed to read section compression");
			if (compression != 2)
				ERROR("Section compression not supported (" << compression << ")");
			NBTBuffer buffer;
			buffer.pos = 0;
			buffer.len = len;
			LOG("len: " << len << " (" << *reinterpret_cast<uint32_t*>(&len) << ")");
			buffer.data = new uint8_t[buffer.len];
			if (std::fread(buffer.data, buffer.len, 1, this->file) != 1)
				ERROR("Invalid region sectors read");
			if (!(NBT = NBTTag::readTag(&buffer)))
			{
				WARN("No NBT found");
			}
			else if (NBT->getType() != NBT_TAG_COMPOUND)
			{
				WARN("Invalid chunk NBT tag: " << (int)NBT->getType());
				delete (NBT);
				NBT = NULL;
			}
			delete[] ((uint8_t*)buffer.data);
		}
		Chunk *chunk = new Chunk(this->world, this->x + x * CHUNK_WIDTH, this->z + z * CHUNK_WIDTH);
		chunk->initNBT(reinterpret_cast<NBTTagCompound*>(NBT));
		setChunk(x, z, chunk);
		return (chunk);
	}

	void Region::setChunk(int32_t x, int32_t z, Chunk *chunk)
	{
		Chunk *oldChunk = this->chunks[getXZId(x, z)];
		this->chunks[getXZId(x, z)] = chunk;
		if (!chunk)
		{
			if (oldChunk)
				saveChunk(oldChunk);
			for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
			{
				if (this->chunks[i])
					return;
			}
			std::vector<Region*> &regions = this->world.getRegions();
			for (std::vector<Region*>::iterator iter = regions.begin(); iter != regions.end(); ++iter)
			{
				if (*iter == this)
				{
					regions.erase(iter);
					delete (this);
					return;
				}
			}
		}
	}

	Chunk *Region::getChunk(int32_t x, int32_t z)
	{
		return (this->chunks[getXZId(x, z)]);
	}

	uint32_t Region::getXZId(int32_t x, int32_t z)
	{
		return (x + z * REGION_WIDTH);
	}

}
