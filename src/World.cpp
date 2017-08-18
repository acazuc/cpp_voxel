#include "World.h"

namespace voxel
{

	World::World()
	{
		this->chunks.push_back(new Chunk(this, 0, 0));
		this->chunks.push_back(new Chunk(this, 16, 0));
		this->chunks.push_back(new Chunk(this, 0, 16));
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
