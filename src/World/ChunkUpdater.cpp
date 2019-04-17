#include "ChunkUpdater.h"
#include "Utils/System.h"
#include "World/World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	bool ChunkUpdater::running = false;

	ChunkUpdater::ChunkUpdater(World *world)
	: thread(nullptr)
	, world(world)
	{
	}

	ChunkUpdater::~ChunkUpdater()
	{
	}

	void ChunkUpdater::start()
	{
		running = true;
		this->thread = new std::thread(run, world);
	}

	void ChunkUpdater::stop()
	{
		running = false;
		if (this->thread)
		{
			this->thread->join();
			delete (this->thread);
			this->thread = nullptr;
		}
	}

	void ChunkUpdater::run(void *data)
	{
		int64_t lastUpdate = nanotime / 1000000000 * 1000000000;
		int64_t updatesCount = 0;
		World &world = *reinterpret_cast<World*>(data);
		while (running)
		{
			if (nanotime - lastUpdate >= 1000000000)
			{
				lastUpdate += 1000000000;
				Main::setChunkUpdates(updatesCount);
				updatesCount = 0;
			}
			{
				std::lock_guard<std::recursive_mutex> lock(world.getChunksMutex());
				if (world.getChunksToUpdate().size() == 0)
					goto wait;
				Chunk *chunk = world.getChunksToUpdate().front();
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
					continue;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(5));
			continue;
		wait:
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	}

}
