#include "World.h"
#include "Entities/DroppedBlock.h"
#include "Blocks/Blocks.h"
#include "Debug.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	World::World()
	: chunkUpdater(this)
	, chunkLoader(this)
	, biomeTempNoise(16, .70, 1334538)
	, biomeRainNoise(16, .70, 1222222339)
	, noise(16, .5, 1337)
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
		this->chunkUpdater.stop();
		this->chunkLoader.stop();
		for (uint32_t i = 0; i < this->regions.size(); ++i)
		{
			this->regions[i]->moveGLBuffersToWorld();
			delete (this->regions[i]);
		}
		clearVBOToDelete();
		clearVAOToDelete();
	}

	void World::save()
	{
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->save();
	}

	void World::tick()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		checkEmptyRegions();
		clearVBOToDelete();
		clearVAOToDelete();
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->tick();
		this->player.tick();
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

	void World::clearVBOToDelete()
	{
		for (uint32_t i = 0; i < this->VBOToDelete.size(); ++i)
			delete (this->VBOToDelete[i]);
		this->VBOToDelete.clear();
	}

	void World::clearVAOToDelete()
	{
		for (uint32_t i = 0; i < this->VAOToDelete.size(); ++i)
			delete (this->VAOToDelete[i]);
		this->VAOToDelete.clear();
	}

	void World::draw()
	{
		std::lock_guard<std::recursive_mutex> lock(this->chunksMutex);
		this->player.update();
		Main::getBlocksShader().program->use();
		Main::getBlocksShader().vLocation->setMat4f(this->player.getViewMat());
		Main::getBlocksShader().mvpLocation->setMat4f(this->player.getViewProjMat());
		Main::getBlocksShader().timeFactorLocation->setVec1f(nanotime / 1000000000.);
		if (this->player.isEyeInWater())
		{
			Vec4 color(0, 0, .05, 1);
			Main::getBlocksShader().program->use();
			Main::getBlocksShader().fogColorLocation->setVec4f(color);
			Main::getBlocksShader().fogDistanceLocation->setVec1f(0);
			Main::getBlocksShader().fogDensityLocation->setVec1f(.2 - .1 * (this->player.getEyeLight() / 15.));
			Main::getEntityShader().program->use();
			Main::getEntityShader().fogColorLocation->setVec4f(color);
			Main::getEntityShader().fogDistanceLocation->setVec1f(0);
			Main::getEntityShader().fogDensityLocation->setVec1f(.2 - .1 * (this->player.getEyeLight() / 15.));
			Main::getDroppedShader().program->use();
			Main::getDroppedShader().fogColorLocation->setVec4f(color);
			Main::getDroppedShader().fogDistanceLocation->setVec1f(0);
			Main::getDroppedShader().fogDensityLocation->setVec1f(.2 - .1 * (this->player.getEyeLight() / 15.));
		}
		else
		{
			Main::getBlocksShader().program->use();
			Main::getBlocksShader().fogColorLocation->setVec4f(Main::getSkyColor());
			Main::getBlocksShader().fogDistanceLocation->setVec1f(16 * 140);
			Main::getBlocksShader().fogDensityLocation->setVec1f(.1);
			Main::getEntityShader().program->use();
			Main::getEntityShader().fogColorLocation->setVec4f(Main::getSkyColor());
			Main::getEntityShader().fogDistanceLocation->setVec1f(16 * 140);
			Main::getEntityShader().fogDensityLocation->setVec1f(.1);
			Main::getDroppedShader().program->use();
			Main::getDroppedShader().fogColorLocation->setVec4f(Main::getSkyColor());
			Main::getDroppedShader().fogDistanceLocation->setVec1f(16 * 140);
			Main::getDroppedShader().fogDensityLocation->setVec1f(.1);
		}
		Main::getTerrain()->bind();
		Main::getBlocksShader().program->use();
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->draw(0);
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->draw(1);
		this->player.draw();
		this->skybox.draw();
		this->clouds.draw();
		for (std::vector<Region*>::iterator iter = this->regions.begin(); iter != this->regions.end(); ++iter)
			(*iter)->drawEntities();
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
		Vec3 p0 = aabb.getP0();
		Vec3 p1 = aabb.getP1();
		int32_t chunkStartX = getChunkCoord(p0.x);
		int32_t chunkEndX = getChunkCoord(p1.x);
		int32_t chunkStartZ = getChunkCoord(p0.z);
		int32_t chunkEndZ = getChunkCoord(p1.z);
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
							ChunkBlock *block = chunk->getBlock(x, y, z);
							if (!block || !block->getType())
								continue;
							Block *blockModel = Blocks::getBlock(block->getType());
							if (!blockModel || !blockModel->isSolid())
								continue;
							Vec3 p0(blockModel->getAABB().getP0());
							p0.x += chunkX + x;
							p0.y += y;
							p0.z += chunkZ + z;
							Vec3 p1(blockModel->getAABB().getP1());
							p1.x += chunkX + x;
							p1.y += y;
							p1.z += chunkZ + z;
							aabbs.push_back(AABB(p0, p1));
						}
					}
				}
			}
		}
	}

	void World::setBlock(int32_t x, int32_t y, int32_t z, uint8_t type)
	{
		if (y < 0 || y >= CHUNK_HEIGHT)
			return;
		int32_t chunkX = getChunkCoord(x);
		int32_t chunkZ = getChunkCoord(z);
		Chunk *chunk = getChunk(chunkX, chunkZ);
		if (!chunk)
		{
			chunk = new Chunk(*this, chunkX, chunkZ);
			addChunk(chunk);
		}
		chunk->setBlock(x - chunkX, y, z - chunkZ, type);
	}

	void World::setBlockIfReplaceable(int32_t x, int32_t y, int32_t z, uint8_t type)
	{
		if (y < 0 || y >= CHUNK_HEIGHT)
			return;
		int32_t chunkX = getChunkCoord(x);
		int32_t chunkZ = getChunkCoord(z);
		Chunk *chunk = getChunk(chunkX, chunkZ);
		if (!chunk)
		{
			chunk = new Chunk(*this, chunkX, chunkZ);
			addChunk(chunk);
		}
		chunk->setBlockIfReplaceable(x - chunkX, y, z - chunkZ, type);
	}

	void World::generateChunk(int32_t x, int32_t z)
	{
		int32_t regionX = getRegionCoord(x);
		int32_t regionZ = getRegionCoord(z);
		for (uint32_t i = 0; i < this->regions.size(); ++i)
		{
			Region *region = this->regions[i];
			if (region->getX() == regionX && region->getZ() == regionZ)
			{
				region->generateChunk((x - regionX) / CHUNK_WIDTH, (z - regionZ) / CHUNK_WIDTH);
				return;
			}
		}
		Region *region = new Region(*this, regionX, regionZ);
		this->regions.push_back(region);
		region->generateChunk((x - regionX) / CHUNK_WIDTH, (z - regionZ) / CHUNK_WIDTH);
	}

	void World::addChunk(Chunk *chunk)
	{
		int32_t x = chunk->getX();
		int32_t z = chunk->getZ();
		int32_t regionX = getRegionCoord(x);
		int32_t regionZ = getRegionCoord(z);
		for (uint32_t i = 0; i < this->regions.size(); ++i)
		{
			Region *region = this->regions[i];
			if (region->getX() == regionX && region->getZ() == regionZ)
			{
				region->setChunk((x - regionX) / CHUNK_WIDTH, (z - regionZ) / CHUNK_WIDTH, chunk);
				return;
			}
		}
		Region *region = new Region(*this, regionX, regionZ);
		this->regions.push_back(region);
		region->setChunk((x - regionX) / CHUNK_WIDTH, (z - regionZ) / CHUNK_WIDTH, chunk);
	}

	Chunk *World::getChunk(int32_t x, int32_t z)
	{
		int32_t regionX = getRegionCoord(x);
		int32_t regionZ = getRegionCoord(z);
		for (uint32_t i = 0; i < this->regions.size(); ++i)
		{
			Region *region = this->regions[i];
			if (region->getX() == regionX && region->getZ() == regionZ)
			{
				return (region->getChunk((x - regionX) / CHUNK_WIDTH, (z - regionZ) / CHUNK_WIDTH));
			}
		}
		return (NULL);
	}

	ChunkBlock *World::getBlock(int32_t x, int32_t y, int32_t z)
	{
		if (y < 0 || y >= CHUNK_HEIGHT)
			return (NULL);
		int32_t chunkX = getChunkCoord(x);
		int32_t chunkZ = getChunkCoord(z);
		Chunk *chunk = getChunk(chunkX, chunkZ);
		if (!chunk)
			return (NULL);
		return (chunk->getBlock(x - chunkX, y, z - chunkZ));
	}

	uint8_t World::getLight(int32_t x, int32_t y, int32_t z)
	{
		if (y < 0 || y >= CHUNK_HEIGHT)
			return (0);
		int32_t chunkX = getChunkCoord(x);
		int32_t chunkZ = getChunkCoord(z);
		Chunk *chunk = getChunk(chunkX, chunkZ);
		if (!chunk)
			return (0);
		return (chunk->getLight(x - chunkX, y, z - chunkZ));
	}

	int32_t World::getChunkCoord(int32_t coord)
	{
		return (coord & (~(CHUNK_WIDTH - 1)));
	}

	int32_t World::getRegionCoord(int32_t coord)
	{
		return (coord & (~(REGION_WIDTH * CHUNK_WIDTH - 1)));
	}

}
