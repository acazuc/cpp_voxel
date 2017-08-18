#include "World.h"

namespace voxel
{

	World::World()
	: noise(512, .5, 1338)
	{
		for (uint16_t x = 0; x < 8; ++x)
		{
			for (uint16_t z = 0; z < 8; ++z)
			{
				this->chunks.push_back(new Chunk(this, x * CHUNK_WIDTH, z * CHUNK_WIDTH));
			}
		}
	}

	World::~World()
	{
		for (uint32_t i = 0; i < this->chunks.size(); ++i)
			delete (this->chunks[i]);
	}

	void World::tick()
	{
		this->player.tick();
	}

	void World::draw()
	{
		for (uint32_t i = 0; i < this->chunks.size(); ++i)
			this->chunks[i]->draw();
	}

	Chunk *World::getChunk(int32_t x, int32_t z)
	{
		for (uint32_t i = 0; i < this->chunks.size(); ++i)
		{
			Chunk *chunk = this->chunks[i];
			if (chunk->getX() == x && chunk->getZ() == z)
				return (chunk);
		}
		return (NULL);
	}

}
