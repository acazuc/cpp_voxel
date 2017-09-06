#include "ChunkLoader.h"
#include "World.h"
#include <ctime>

# define LOAD_DISTANCE 16

namespace voxel
{

	bool ChunkLoader::running;

	ChunkLoader::ChunkLoader(World *world)
	{
		running = true;
		this->thread = new std::thread(_run, world);
	}

	ChunkLoader::~ChunkLoader()
	{
		running = false;
		this->thread->join();
	}

	bool ChunkLoader::checkChunk(World &world, int32_t chunkX, int32_t chunkZ)
	{
		if (!running)
			return (true);
		double part1 = (world.getPlayer().getPosZ() - ((int32_t)world.getPlayer().getPosZ() % CHUNK_WIDTH)) - chunkZ;
		double part2 = (world.getPlayer().getPosX() - ((int32_t)world.getPlayer().getPosX() % CHUNK_WIDTH)) - chunkX;
		int32_t distance = sqrt(part1 * part1 + part2 * part2);
		if (distance > LOAD_DISTANCE * CHUNK_WIDTH)
			return (false);
		if (world.getChunk(chunkX, chunkZ))
			return (false);
		std::lock_guard<std::recursive_mutex> lock_guard(world.getChunksMutex());
		Chunk *chunk = new Chunk(world, chunkX, chunkZ);
		world.addChunk(chunk);
		return (true);
	}

	void ChunkLoader::_run(void *data)
	{
		World &world = *reinterpret_cast<World*>(data);
		struct timespec ts;
		ts.tv_sec = 0;
		while (running)
		{
			float playerX = world.getPlayer().getPosX();
			float playerZ = world.getPlayer().getPosZ();
			int32_t playerChunkX = playerX - (int32_t)playerX % CHUNK_WIDTH;
			int32_t playerChunkZ = playerZ - (int32_t)playerZ % CHUNK_WIDTH;
			{
				std::vector<Chunk*> &chunks = world.getChunks();
				for (uint32_t i = 0; i < chunks.size(); ++i)
				{
					Chunk *chunk = chunks[i];
					double part1 = playerChunkZ - (chunk->getZ() + CHUNK_WIDTH / 2);
					double part2 = playerChunkX - (chunk->getX() + CHUNK_WIDTH / 2);
					int distance = sqrt(part1 * part1 + part2 * part2);
					if (distance > LOAD_DISTANCE * 1.5 * CHUNK_WIDTH)
					{
						{
							std::lock_guard<std::recursive_mutex> lock(world.getChunksMutex());
							chunks.erase(chunks.begin() + i);
						}
						world.getBuffersToDelete().push_back(chunk->getTexCoordsBuffer());
						world.getBuffersToDelete().push_back(chunk->getVertexesBuffer());
						world.getBuffersToDelete().push_back(chunk->getIndicesBuffer());
						world.getBuffersToDelete().push_back(chunk->getColorsBuffer());
						chunk->setTexCoordsBuffer(NULL);
						chunk->setVertexesBuffer(NULL);
						chunk->setIndicesBuffer(NULL);
						chunk->setColorsBuffer(NULL);
						delete (chunk);
						i--;
					}
				}
			}
			if (checkChunk(world, playerChunkX, playerChunkZ))
				goto end;
			for (int32_t i = 0; i <= LOAD_DISTANCE; ++i)
			{
				int32_t chunkX = playerChunkX - i * CHUNK_WIDTH;
				int32_t chunkZ = playerChunkZ - i * CHUNK_WIDTH;
				if (checkChunk(world, chunkX, chunkZ))
					goto end;
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkX += CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						goto end;
				}
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkZ += CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						goto end;
				}
				for (int32_t j = 0; j <= i * 2; ++j)
				{
					chunkX -= CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						goto end;
				}
				for (int32_t j = 0; j <= i * 2 - 1; ++j)
				{
					chunkZ -= CHUNK_WIDTH;
					if (checkChunk(world, chunkX, chunkZ))
						goto end;
				}
			}
			ts.tv_nsec = 100000000;
			nanosleep(&ts, NULL);
		end:
			continue;
			ts.tv_nsec = 10000000;
			nanosleep(&ts, NULL);
		}
	}

}
