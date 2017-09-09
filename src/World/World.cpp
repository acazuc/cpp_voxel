#include "World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	World::World()
	: particlesManager(*this)
	, entitiesManager(*this)
	, chunkLoader(this)
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
		this->entitiesManager.tick();
		this->particlesManager.tick();
	}

	void World::draw()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		for (uint32_t i = 0; i < this->buffersToDelete.size(); ++i)
			delete (this->buffersToDelete[i]);
		this->buffersToDelete.clear();
		this->player.update();
		glm::mat4 mvp = this->player.getProjMat() * this->player.getViewMat();
		Main::getBlocksShader().program->use();
		Main::getBlocksShader().vLocation->setMat4f(this->player.getViewMat());
		Main::getBlocksShader().mvpLocation->setMat4f(mvp);
		Main::getBlocksShader().timeFactorLocation->setVec1f(nanotime / 1000000000.);
		Main::getBlocksShader().fogColorLocation->setVec4f(Main::getSkyColor());
		Main::getTerrain()->bind();
		for (std::vector<Chunk*>::iterator iter = this->chunks.begin(); iter != this->chunks.end(); ++iter)
		{
			Chunk *chunk = *iter;
			chunk->draw();
		}
		this->particlesManager.draw();
		this->entitiesManager.draw();
		this->clouds.draw();
		this->skybox.draw();
		this->player.draw();
	}

	void World::getAABBs(AABB &aabb, std::vector<AABB> &aabbs)
	{
		if (aabb.getP1().y < 0 || aabb.getP0().y >= CHUNK_HEIGHT)
			return;
		glm::vec3 p0 = aabb.getP0();
		glm::vec3 p1 = aabb.getP1();
		int32_t chunkStartX = std::floor(p0.x / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkEndX = std::floor(p1.x / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkStartZ = std::floor(p0.z / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkEndZ = std::floor(p1.z / CHUNK_WIDTH) * CHUNK_WIDTH;
		for (int32_t chunkX = chunkStartX; chunkX <= chunkEndX; ++chunkX)
		{
			for (int32_t chunkZ = chunkStartZ; chunkZ <= chunkEndZ; ++chunkZ)
			{
				Chunk *chunk = getChunk(chunkX, chunkZ);
				if (!chunk)
					continue;
				int32_t startX = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(p0.x - chunkX)));
				int32_t startY = std::max(0, std::min(CHUNK_HEIGHT - 1, static_cast<int32_t>(p0.y)));
				int32_t startZ = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(p0.z - chunkZ)));
				int32_t endX = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(p1.x - chunkX)));
				int32_t endY = std::max(0, std::min(CHUNK_HEIGHT - 1, static_cast<int32_t>(p1.y)));
				int32_t endZ = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(p1.z - chunkZ)));
				for (int32_t x = startX; x <= endX; ++x)
				{
					for (int32_t y = startY; y <= endY; ++y)
					{
						for (int32_t z = startZ; z <= endZ; ++z)
						{
							float s = BLOCK_SIZE;
							if (!chunk->getBlockAt(glm::vec3(x, y, z))->getType())
								continue;
							aabbs.push_back(AABB(glm::vec3(chunkX + x, y, chunkZ + z), glm::vec3(chunkX + x + s, y + s, chunkZ + z + s)));
						}
					}
				}
			}
		}
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