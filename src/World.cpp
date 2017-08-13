#include "World.h"

namespace voxel
{

	World::World()
	{
		this->chunks.push_back(new Chunk(this, 0, 0));
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

}
