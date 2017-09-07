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

	void World::getAABBs(AABB &aabb, std::vector<AABB> &aabbs)
	{
		if (aabb.getY1() < 0 || aabb.getY0() >= CHUNK_HEIGHT)
			return;
		float x0 = aabb.getX0();
		float y0 = aabb.getY0();
		float z0 = aabb.getZ0();
		float x1 = aabb.getX1();
		float y1 = aabb.getY1();
		float z1 = aabb.getZ1();
		int32_t chunkStartX = std::floor(x0 / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkEndX = std::floor(x1 / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkStartZ = std::floor(z0 / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkEndZ = std::floor(z1 / CHUNK_WIDTH) * CHUNK_WIDTH;
		for (int32_t chunkX = chunkStartX; chunkX <= chunkEndX; ++chunkX)
		{
			for (int32_t chunkZ = chunkStartZ; chunkZ <= chunkEndZ; ++chunkZ)
			{
				Chunk *chunk = getChunk(chunkX, chunkZ);
				if (!chunk)
					continue;
				int32_t startX = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(x0 - chunkX)));
				int32_t startY = std::max(0, std::min(CHUNK_HEIGHT - 1, static_cast<int32_t>(y0)));
				int32_t startZ = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(z0 - chunkZ)));
				int32_t endX = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(x1 - chunkX)));
				int32_t endY = std::max(0, std::min(CHUNK_HEIGHT - 1, static_cast<int32_t>(y1)));
				int32_t endZ = std::max(0, std::min(CHUNK_WIDTH - 1, static_cast<int32_t>(z1 - chunkZ)));
				for (int32_t x = startX; x <= endX; ++x)
				{
					for (int32_t y = startY; y <= endY; ++y)
					{
						for (int32_t z = startZ; z <= endZ; ++z)
						{
							float s = BLOCK_SIZE;
							if (!chunk->getBlockAt(x, y, z)->getType())
								continue;
							aabbs.push_back(AABB(chunkX + x, y, chunkZ + z, chunkX + x + s, y + s, chunkZ + z + s));
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
