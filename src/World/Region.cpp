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
	{
		std::memset(this->chunks, 0, sizeof(this->chunks));
	}

	Region::~Region()
	{
		for (uint32_t i = 0; i < REGION_WIDTH * REGION_WIDTH; ++i)
			delete (this->chunks[i]);
	}

	void Region::save()
	{
		//
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
		if (chunk)
		{
			chunk->generate();
			return;
		}
		chunk = new Chunk(this->world, this->x + x * CHUNK_WIDTH, this->z + z * CHUNK_WIDTH);
		setChunk(x, z, chunk);
		chunk->generate();
	}

	void Region::setChunk(int32_t x, int32_t z, Chunk *chunk)
	{
		this->chunks[x * REGION_WIDTH + z] = chunk;
	}

	Chunk *Region::getChunk(int32_t x, int32_t z)
	{
		return (this->chunks[x * REGION_WIDTH + z]);
	}

}
