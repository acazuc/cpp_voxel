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
		Main::getBlocksShader().program->use();
		Main::getBlocksShader().vLocation->setMat4f(this->player.getViewMat());
		Main::getBlocksShader().mvpLocation->setMat4f(mvp);
		Main::getBlocksShader().timeFactorLocation->setVec1f(nanotime / 1000000000.);
		Main::getBlocksShader().fogColorLocation->setVec4f(Main::getSkyColor());
		glBindTexture(GL_TEXTURE_2D, Main::getTerrain()->getTextureID());
		for (std::vector<Chunk*>::iterator iter = this->chunks.begin(); iter != this->chunks.end(); ++iter)
		{
			Chunk *chunk = *iter;
			if (chunk->isDeleted())
			{
				delete (chunk);
				--iter;
				this->chunks.erase(iter + 1);
				continue;
			}
			chunk->draw();
		}
		this->chunksMutex.unlock();
		this->clouds.draw();
		this->skybox.draw();
		this->player.draw();
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
