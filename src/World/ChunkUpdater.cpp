#include "ChunkUpdater.h"
#include "Utils/System.h"
#include "World/World.h"

namespace voxel
{

	bool ChunkUpdater::running;

	ChunkUpdater::ChunkUpdater(World *world)
	{
		running = true;
		this->thread = new std::thread(run, world);
	}

	ChunkUpdater::~ChunkUpdater()
	{
		running = false;
		this->thread->join();
	}

	void ChunkUpdater::run(void *data)
	{
		World &world = *reinterpret_cast<World*>(data);
		while (running)
		{
			{
			start:
				std::lock_guard<std::recursive_mutex> lock(world.getChunksMutex());
				if (world.getChunksToUpdate().size() == 0)
					goto end;
				Chunk *chunk = world.getChunksToUpdate().front();
				world.getChunksToUpdate().pop_front();
				if (chunk->isMustGenerateLightMap())
				{
					chunk->generateLightMap();
					chunk->generateBuffers();
				}
				else if (chunk->isMustGenerateBuffers())
				{
					chunk->generateBuffers();
				}
				else
					goto start;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(5));
			continue;
		end:
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

}
