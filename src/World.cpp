#include "World.h"
#include "Main.h"

namespace voxel
{

	World::World()
	: chunkLoader(this)
	, noise(512, .5, 1337)
	, frustum(*this)
	, player(*this)
	{
		//Empty
	}

	World::~World()
	{
		for (uint32_t i = 0; i < this->chunks.size(); ++i)
			delete (this->chunks[i]);
	}

	void World::tick()
	{
		this->chunksMutex.lock();
		this->player.tick();
		this->chunksMutex.unlock();
	}

	void World::draw()
	{
		this->chunksMutex.lock();
		glm::mat4 mvp = this->player.getProjMat() * this->player.getViewMat();
		Main::getMvpLocation()->setMat4f(mvp);
		for (uint32_t i = 0; i < this->chunks.size(); ++i)
			this->chunks[i]->draw();
		this->chunksMutex.unlock();
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

	void World::addChunk(Chunk *chunk)
	{
		this->chunks.push_back(chunk);
	}

}
