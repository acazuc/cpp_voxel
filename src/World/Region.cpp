#include "Region.h"
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
		this->file = std::fopen(filename.c_str(), "ab");
		if (!this->file)
			ERROR("Failed to open region file \"" << this->filename << "\"");
		if (std::fread(this->storageHeader, sizeof(this->storageHeader), 1, this->file) != 1)
		{
			WARN("Failed to read header of \"" << this->filename << "\"");
			return;
		}
		if (!std::fseek(this->file, 0, SEEK_END))
			ERROR("Failed to get size of file \"" << this->filename << "\"");
		int64_t filesize = std::ftell(this->file);
		if (filesize == -1)
			ERROR("Failed to ftell \"" << this->filename << "\"");
		if (filesize & 0xff)
		{
			char c = 0;
			for (uint32_t i = 0; i < filesize; ++i)
			{
				if (std::fwrite(&c, 1, 1, this->file) != 1)
					ERROR("Failed to grow region file");
				++filesize;
			}
		}
		this->sectors.resize((filesize - sizeof(this->storageHeader)) / REGION_SECTOR_SIZE, false);
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			uint32_t offsetAlloc = this->storageHeader[i];
			uint32_t offset = (offsetAlloc >> 1) & 0xffffff;
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
		bool changed = false;
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			if (!this->chunks[i])
				continue;
			if (!this->chunks[i]->isChanged())
				continue;
			this->chunks[i]->setChanged(false);
			char *data = NULL;
			uint32_t dataLen = 0;
			uint32_t sectorsLen = std::ceil(dataLen / (float)REGION_SECTOR_SIZE);
			if (!sectorsLen)
				continue;
			uint32_t &offsetAlloc = this->storageHeader[i];
			uint32_t offset = (offsetAlloc >> 8) & 0xffffff;
			uint8_t allocated = offsetAlloc & 0xff;
			if (!offset || allocated < sectorsLen)
			{
				for (uint32_t a = 0; a < allocated; ++a)
					this->sectors[a + offset] = false;
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
					goto write;
nextTest:
					continue;
				}
				offset = this->sectors.size();
				allocated = sectorsLen;
				offsetAlloc = ((offset & 0xffffff) << 8) | allocated;
				this->sectors.resize(this->sectors.size() + allocated, true);
			}
write:
			changed = true;
			std::fseek(this->file, sizeof(this->storageHeader) + offset * REGION_SECTOR_SIZE, SEEK_SET);
			if (std::fwrite(data, sectorsLen * REGION_SECTOR_SIZE, 1, this->file) != 1)
				WARN("Failed to write region chunk storage");
		}
		if (changed)
		{
			std::fseek(this->file, 0, SEEK_SET);
			if (std::fwrite(this->storageHeader, sizeof(this->storageHeader), 1, this->file) != 1)
				WARN("Failed to write region \"" << this->filename << "\" offsets");
		}
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
		{
			chunk = new Chunk(this->world, this->x + x * CHUNK_WIDTH, this->z + z * CHUNK_WIDTH);
			setChunk(x, z, chunk);
		}
		chunk->generate();
	}

	void Region::setChunk(int32_t x, int32_t z, Chunk *chunk)
	{
		this->chunks[x * REGION_WIDTH + z] = chunk;
		if (!chunk)
		{
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
				}
			}
		}
	}

	Chunk *Region::getChunk(int32_t x, int32_t z)
	{
		return (this->chunks[x * REGION_WIDTH + z]);
	}

}
