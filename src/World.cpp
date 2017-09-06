#include "World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	World::World()
	: chunkLoader(this)
	, noise(512, .5, 1337)
	, frustum(*this)
	, player(*this)
	, clouds(*this)
	, skybox(*this)
	{
		this->chunks.reserve(16 * 16);
	}

	World::~World()
	{
		for (uint32_t i = 0; i < this->chunks.size(); ++i)
			delete (this->chunks[i]);
	}

	void World::update()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		for (uint32_t i = 0; i < this->chunks.size(); ++i)
			this->chunks[i]->regenerateLightMap();
	}

	void World::tick()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		this->player.tick();
	}

	void World::draw()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		for (uint32_t i = 0; i < this->buffersToDelete.size(); ++i)
			delete (this->buffersToDelete[i]);
		this->buffersToDelete.clear();
		glm::mat4 mvp = this->player.getProjMat() * this->player.getViewMat();
		Main::getBlocksShader().program->use();
		Main::getBlocksShader().vLocation->setMat4f(this->player.getViewMat());
		Main::getBlocksShader().mvpLocation->setMat4f(mvp);
		Main::getBlocksShader().timeFactorLocation->setVec1f(nanotime / 1000000000.);
		Main::getBlocksShader().fogColorLocation->setVec4f(Main::getSkyColor());
		glBindTexture(GL_TEXTURE_2D, Main::getTerrain()->getId());
		for (std::vector<Chunk*>::iterator iter = this->chunks.begin(); iter != this->chunks.end(); ++iter)
		{
			Chunk *chunk = *iter;
			chunk->draw();
		}
		this->clouds.draw();
		this->skybox.draw();
		this->player.draw();
	}

	Chunk *World::getChunk(int32_t x, int32_t z)
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
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
