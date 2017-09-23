#include "World.h"
#include "Blocks/Blocks.h"
#include "Debug.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	World::World()
	: particlesManager(*this)
	, entitiesManager(*this)
	, chunkUpdater(this)
	, chunkLoader(this)
	, noise(512, .5, 1337)
	, frustum(*this)
	, player(*this)
	, clouds(*this)
	, skybox(*this)
	, lastRegionCheck(nanotime)
	{
		this->random.seed(1337);
		this->chunkUpdater.start();
		this->chunkLoader.start();
	}

	World::~World()
	{
		for (uint32_t i = 0; i < this->regions.size(); ++i)
			delete (this->regions[i]);
	}

	void World::update()
	{
		//std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
	}

	void World::tick()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		checkEmptyRegions();
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->tick();
		this->player.tick();
		this->entitiesManager.tick();
		this->particlesManager.tick();
		this->clouds.tick();
	}

	void World::checkEmptyRegions()
	{
		if (nanotime - this->lastRegionCheck < 5000000000)
			return;
		this->lastRegionCheck = nanotime;
		for (uint32_t i = 0; i < this->regions.size(); ++i)
		{
			Region *region = this->regions[i];
			Chunk **chunks = region->getChunks();
			for (uint32_t j = 0; j < REGION_WIDTH * REGION_WIDTH; ++j)
			{
				if (chunks[j])
					goto nextRegion;
			}
			this->regions.erase(this->regions.begin() + i);
			--i;
nextRegion:
			continue;
		}
	}

	void World::draw()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		this->player.update();
		glm::mat4 mvp = this->player.getViewProjMat();
		Main::getBlocksShader().program->use();
		Main::getBlocksShader().vLocation->setMat4f(this->player.getViewMat());
		Main::getBlocksShader().mvpLocation->setMat4f(mvp);
		Main::getBlocksShader().timeFactorLocation->setVec1f(nanotime / 1000000000.);
		if (this->player.isEyeInWater())
		{
			glm::vec4 color(0, 0, .075, 1);
			Main::getBlocksShader().fogColorLocation->setVec4f(color);
			Main::getBlocksShader().fogDistanceLocation->setVec1f(0);
			Main::getBlocksShader().fogDensityLocation->setVec1f(.2 - .1 * (this->player.getEyeLight() / 15.));
		}
		else
		{
			Main::getBlocksShader().fogColorLocation->setVec4f(Main::getSkyColor());
			Main::getBlocksShader().fogDistanceLocation->setVec1f(16 * 12);
			Main::getBlocksShader().fogDensityLocation->setVec1f(.1);
		}
		Main::getTerrain()->bind();
		for (uint32_t i = 0; i < this->buffersToDelete.size(); ++i)
			delete (this->buffersToDelete[i]);
		this->buffersToDelete.clear();
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->draw(0);
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->draw(1);
		this->player.draw();
		this->particlesManager.draw();
		this->entitiesManager.draw();
		this->skybox.draw();
		this->clouds.draw();
		Main::getBlocksShader().program->use();
		Main::getTerrain()->bind();
		glDisable(GL_CULL_FACE);
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->draw(2);
		glEnable(GL_CULL_FACE);
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
		for (int32_t chunkX = chunkStartX; chunkX <= chunkEndX; chunkX += CHUNK_WIDTH)
		{
			for (int32_t chunkZ = chunkStartZ; chunkZ <= chunkEndZ; chunkZ += CHUNK_WIDTH)
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
							ChunkBlock *block = chunk->getBlock(glm::vec3(x, y, z));
							if (!block || !block->getType())
								continue;
							Block *blockModel = Blocks::getBlock(block->getType());
							if (!blockModel || !blockModel->isSolid())
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
		for (uint32_t i = 0; i < this->regions.size(); ++i)
		{
			Region *region = this->regions[i];
			int32_t regionX = region->getX();
			int32_t regionZ = region->getZ();
			if (x >= regionX && x < regionX + REGION_WIDTH && z >= regionZ && z < regionZ + REGION_WIDTH)
				return (region->getChunk((x - region->getX()) / CHUNK_WIDTH, (z - region->getZ()) / CHUNK_WIDTH));
		}
		return (NULL);
	}

	void World::addChunk(Chunk *chunk)
	{
		int32_t regionX = std::floor((float)chunk->getX() / REGION_WIDTH) * REGION_WIDTH;
		int32_t regionZ = std::floor((float)chunk->getZ() / REGION_WIDTH) * REGION_WIDTH;
		for (uint32_t i = 0; i < this->regions.size(); ++i)
		{
			Region *region = this->regions[i];
			if (region->getX() == regionX && region->getZ() == regionZ)
			{
				region->setChunk((chunk->getX() - region->getX()) / CHUNK_WIDTH, (chunk->getZ() - region->getZ()) / CHUNK_WIDTH, chunk);
				return;
			}
		}
		Region *region = new Region(*this, regionX, regionZ);
		region->setChunk((chunk->getX() - region->getX()) / CHUNK_WIDTH, (chunk->getZ() - region->getZ()) / CHUNK_WIDTH, chunk);
		this->regions.push_back(region);
	}

	ChunkBlock *World::getBlock(glm::vec3 pos)
	{
		if (pos.y < 0 || pos.y >= CHUNK_HEIGHT)
			return (NULL);
		int32_t chunkX = std::floor(pos.x / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkZ = std::floor(pos.z / CHUNK_WIDTH) * CHUNK_WIDTH;
		Chunk *chunk = getChunk(chunkX, chunkZ);
		if (!chunk)
			return (NULL);
		return (chunk->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ())));
	}

	uint8_t World::getLight(glm::vec3 pos)
	{
		ChunkBlock *block = getBlock(pos);
		if (!block)
			return (15);
		return (block->getLight());
		/*if (pos.y < 0 || pos.y >= CHUNK_HEIGHT)
			return (15);
		int32_t chunkX = std::floor(pos.x / CHUNK_WIDTH) * CHUNK_WIDTH;
		int32_t chunkZ = std::floor(pos.z / CHUNK_WIDTH) * CHUNK_WIDTH;
		Chunk *chunk = getChunk(chunkX, chunkZ);
		if (!chunk)
			return (15);
		return (chunk->getLightAt(glm::vec3(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ())));*/
	}

}
