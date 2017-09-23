#include "ChunkUpdater.h"
#include "Utils/System.h"
#include "World/World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	bool ChunkUpdater::running = false;

	ChunkUpdater::ChunkUpdater(World *world)
	: thread(NULL)
	, world(world)
	{
		//Empty
	}

	ChunkUpdater::~ChunkUpdater()
	{
		running = false;
		if (this->thread)
			this->thread->join();
	}

	void ChunkUpdater::start()
	{
		running = true;
		this->thread = new std::thread(run, world);
	}

	void ChunkUpdater::run(void *data)
	{
		int64_t lastUpdate = nanotime / 1000000000 * 1000000000;
		int64_t updatesCount = 0;
		World &world = *reinterpret_cast<World*>(data);
		while (running)
		{
			{
			start:
				if (nanotime - lastUpdate >= 1000000000)
				{
					lastUpdate += 1000000000;
					Main::setChunkUpdates(updatesCount);
					updatesCount = 0;
				}
				if (world.getChunksToUpdate().size() == 0)
					goto end;
				Chunk *chunk = world.getChunksToUpdate().front();
				std::lock_guard<std::recursive_mutex> lock1(world.getChunksToUpdateMutex());
				std::lock_guard<std::recursive_mutex> lock2(world.getChunksMutex());
				world.getChunksToUpdate().pop_front();
				if (chunk->isMustGenerateLightMap())
				{
					++updatesCount;
					chunk->generateLightMap();
					chunk->generateBuffers();
				}
				else if (chunk->isMustGenerateBuffers())
				{
					++updatesCount;
					chunk->generateBuffers();
				}
				else
					goto start;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			continue;
		end:
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

}
