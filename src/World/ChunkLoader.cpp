#include "ChunkLoader.h"
#include "Utils/System.h"
#include "World/World.h"
#include "Debug.h"

#define LOAD_DISTANCE 16

#define ON_LOADED {}//std::this_thread::sleep_for(std::chrono::nanoseconds(1));}

namespace voxel
{

	bool ChunkLoader::running = false;

	ChunkLoader::ChunkLoader(World *world)
	: thread(NULL)
	, world(world)
	{
		//Empty
	}

	ChunkLoader::~ChunkLoader()
	{
		//Empty
	}

	void ChunkLoader::start()
	{
		running = true;
		this->thread = new std::thread(run, this->world);
	}

	void ChunkLoader::stop()
	{
		running = false;
		if (this->thread)
		{
			this->thread->join();
			delete (this->thread);
			this->thread = NULL;
		}
	}

	bool ChunkLoader::checkChunk(World &world, Frustum &frustum, int32_t chunkX, int32_t chunkZ)
	{
		if (!running)
			return (true);
		int32_t part1 = world.getPlayer().getPos().x - (chunkX + CHUNK_WIDTH / 2);
		int32_t part2 = world.getPlayer().getPos().z - (chunkZ + CHUNK_WIDTH / 2);
		int32_t distance = sqrt(part1 * part1 + part2 * part2);
		if (distance > LOAD_DISTANCE * CHUNK_WIDTH)
			return (false);
		if (distance > 2 * 16)
		{
			AABB aabb(glm::vec3(chunkX, 0, chunkZ), glm::vec3(chunkX + CHUNK_WIDTH, CHUNK_HEIGHT, chunkZ + CHUNK_WIDTH));
			if (!frustum.check(aabb))
				return (false);
		}
		std::lock_guard<std::recursive_mutex> lock_guard(world.getChunksMutex());
		world.generateChunk(chunkX, chunkZ);
		return (true);
	}

	void ChunkLoader::run(void *data)
	{
		Frustum frustum;
		World &world = *reinterpret_cast<World*>(data);
		int32_t playerX;
		int32_t playerZ;
		while (running)
		{
			{
				std::lock_guard<std::recursive_mutex> lock(world.getChunksMutex());
				playerX = world.getPlayer().getPos().x;
				playerZ = world.getPlayer().getPos().z;
				frustum.update(world.getPlayer().getViewProjMat());
			}
			int32_t playerChunkX = World::getChunkCoord(playerX);
			int32_t playerChunkZ = World::getChunkCoord(playerZ);
			{
				std::vector<Region*> &regions = world.getRegions();
				for (uint32_t i = 0; i < regions.size(); ++i)
				{
					Region *region = regions[i];
					Chunk **chunks = region->getChunks();
					for (uint32_t j = 0; j < REGION_WIDTH * REGION_WIDTH; ++j)
					{
						Chunk *chunk = chunks[j];
						if (!chunk)
							continue;
						int32_t part1 = playerX - (chunk->getX() + CHUNK_WIDTH / 2);
						int32_t part2 = playerZ - (chunk->getZ() + CHUNK_WIDTH / 2);
						int32_t distance = sqrt(part1 * part1 + part2 * part2);
						if (distance < LOAD_DISTANCE * 1.5 * CHUNK_WIDTH)
							continue;
						{
							std::lock_guard<std::recursive_mutex> lock(world.getChunksMutex());
							chunk->moveGLBuffersToWorld();
							region->setChunk((chunk->getX() - region->getX()) / CHUNK_WIDTH, (chunk->getZ() - region->getZ()) / CHUNK_WIDTH, NULL);
							for (std::list<Chunk*>::iterator iter = world.getChunksToUpdate().begin(); iter != world.getChunksToUpdate().end(); ++iter)
							{
								if (*iter != chunk)
									continue;
								world.getChunksToUpdate().erase(iter);
								break;
							}
							delete (chunk);
						}
					}
				}
			}
			if (checkChunk(world, frustum, playerChunkX, playerChunkZ))
				ON_LOADED;
			for (int32_t i = 0; i <= LOAD_DISTANCE; ++i)
			{
				int32_t chunkX = playerChunkX - i * CHUNK_WIDTH;
				int32_t chunkZ = playerChunkZ - i * CHUNK_WIDTH;
				if (checkChunk(world, frustum, chunkX, chunkZ))
					ON_LOADED;
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkX += CHUNK_WIDTH;
					if (checkChunk(world, frustum, chunkX, chunkZ))
						ON_LOADED;
				}
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkZ += CHUNK_WIDTH;
					if (checkChunk(world, frustum, chunkX, chunkZ))
						ON_LOADED;
				}
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkX -= CHUNK_WIDTH;
					if (checkChunk(world, frustum, chunkX, chunkZ))
						ON_LOADED;
				}
				for (int32_t j = 0; j <= i * 2 - 1; ++j)
				{
					chunkZ -= CHUNK_WIDTH;
					if (checkChunk(world, frustum, chunkX, chunkZ))
						ON_LOADED;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

}
