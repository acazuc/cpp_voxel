#include "Region.h"
#include "World.h"
#include <cstring>

namespace voxel
{

	Region::Region(World &world, int32_t x, int32_t z)
	: world(world)
	, x(x)
	, z(z)
	{
		this->chunks = new Chunk*[REGION_WIDTH * REGION_WIDTH];
		std::memset(this->chunks, 0, sizeof(*this->chunks) * REGION_WIDTH * REGION_WIDTH);
	}

	Region::~Region()
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
			delete (this->chunks[i]);
		delete[] (this->chunks);
	}

	void Region::tick()
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
		{
			if (this->chunks[i])
				this->chunks[i]->tick();
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

	void Region::setChunk(int32_t x, int32_t z, Chunk *chunk)
	{
		Chunk *currentChunk = this->chunks[x * REGION_WIDTH + z];
		if (currentChunk)
		{
		begin:
			for (std::list<Chunk*>::iterator iter = this->world.getChunksToUpdate().begin(); iter != this->world.getChunksToUpdate().end(); ++iter)
			{
				if (*iter == currentChunk)
				{
					if (iter == this->world.getChunksToUpdate().begin())
					{
						this->world.getChunksToUpdate().erase(iter);
						goto begin;
					}
					else if (iter == this->world.getChunksToUpdate().end())
					{
						this->world.getChunksToUpdate().erase(iter);
						break;
					}
					else
					{
						std::list<Chunk*>::iterator prev = iter;
						--prev;
						this->world.getChunksToUpdate().erase(iter);
						iter = prev;
					}
				}
			}
			delete (currentChunk);
		}
		this->chunks[x * REGION_WIDTH + z] = chunk;
	}

	Chunk *Region::getChunk(int32_t x, int32_t z)
	{
		return (this->chunks[x * REGION_WIDTH + z]);
	}

}
