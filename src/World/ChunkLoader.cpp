#include "ChunkLoader.h"
#include "Utils/System.h"
#include "World/World.h"
#include "Debug.h"

#define LOAD_DISTANCE 16

#define ON_LOADED {};//if (++loadedChunks > 50) {goto end;}}

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
		running = false;
		if (this->thread)
			this->thread->join();
	}

	void ChunkLoader::start()
	{
		running = true;
		this->thread = new std::thread(run, this->world);
	}

	bool ChunkLoader::checkChunk(World &world, int32_t chunkX, int32_t chunkZ)
	{
		if (!running)
			return (true);
		double part1 = (world.getPlayer().getPos().z - ((int32_t)world.getPlayer().getPos().z % CHUNK_WIDTH)) - chunkZ;
		double part2 = (world.getPlayer().getPos().x - ((int32_t)world.getPlayer().getPos().x % CHUNK_WIDTH)) - chunkX;
		int32_t distance = sqrt(part1 * part1 + part2 * part2);
		if (distance > LOAD_DISTANCE * CHUNK_WIDTH)
			return (false);
		if (distance > 2 * 16)
		{
			AABB aabb(glm::vec3(chunkX, 0, chunkZ), glm::vec3(chunkX + CHUNK_WIDTH, CHUNK_HEIGHT, chunkZ + CHUNK_WIDTH));
			if (!world.getFrustum().check(aabb))
				return (false);
		}
		std::lock_guard<std::recursive_mutex> lock_guard(world.getChunksMutex());
		if (world.getChunk(chunkX, chunkZ))
			return (false);
		Chunk *chunk = new Chunk(world, chunkX, chunkZ);
		world.addChunk(chunk);
		return (true);
	}

	void ChunkLoader::run(void *data)
	{
		World &world = *reinterpret_cast<World*>(data);
		int64_t lastCheck = 0;
		while (running)
		{
			float playerX = world.getPlayer().getPos().x;
			float playerZ = world.getPlayer().getPos().z;
			int32_t playerChunkX = std::floor(playerX / CHUNK_WIDTH) * CHUNK_WIDTH;
			int32_t playerChunkZ = std::floor(playerZ / CHUNK_WIDTH) * CHUNK_WIDTH;
			int64_t nanotime = System::nanotime();
			if (nanotime - lastCheck > 2000000000)
			{
				lastCheck = nanotime;
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
						double part1 = playerChunkZ - (chunk->getZ() + CHUNK_WIDTH / 2);
						double part2 = playerChunkX - (chunk->getX() + CHUNK_WIDTH / 2);
						int32_t distance = sqrt(part1 * part1 + part2 * part2);
						if (distance > LOAD_DISTANCE * 1.5 * CHUNK_WIDTH)
						{
							std::lock_guard<std::recursive_mutex> lock(world.getChunksMutex());
							for (uint8_t i = 0; i < 3; ++i)
							{
								ChunkLayer &layer = chunk->getLayer(i);
								world.getBuffersToDelete().push_back(layer.texCoordsBuffer);
								world.getBuffersToDelete().push_back(layer.vertexesBuffer);
								world.getBuffersToDelete().push_back(layer.indicesBuffer);
								world.getBuffersToDelete().push_back(layer.colorsBuffer);
								layer.texCoordsBuffer = NULL;
								layer.vertexesBuffer = NULL;
								layer.indicesBuffer = NULL;
								layer.colorsBuffer = NULL;
							}
							regions[i]->setChunk((chunk->getX() - region->getX()) / CHUNK_WIDTH, (chunk->getZ() - region->getZ()) / CHUNK_WIDTH, NULL);
						}
					}
				}
			}
			//uint8_t loadedChunks = 0;
			if (checkChunk(world, playerChunkX, playerChunkZ))
				ON_LOADED;
			for (int32_t i = 0; i <= LOAD_DISTANCE; ++i)
			{
				int32_t chunkX = playerChunkX - i * CHUNK_WIDTH;
				int32_t chunkZ = playerChunkZ - i * CHUNK_WIDTH;
				if (checkChunk(world, chunkX, chunkZ))
					ON_LOADED;
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkX += CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						ON_LOADED;
				}
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkZ += CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						ON_LOADED;
				}
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkX -= CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						ON_LOADED;
				}
				for (int32_t j = 0; j <= i * 2 - 1; ++j)
				{
					chunkZ -= CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						ON_LOADED;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//end:
			continue;
		}
	}

}
