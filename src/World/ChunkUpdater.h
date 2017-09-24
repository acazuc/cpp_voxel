#ifndef CHUNK_UPDATER_H
# define CHUNK_UPDATER_H

# include "Platform.h"
# include <cstdint>
# ifdef PLATFORM_WINDOWS
#  include <thread.h>
# else
#  include <thread>
# endif

namespace voxel
{

	class World;

	class ChunkUpdater
	{

	private:
		static bool running;
		static void run(void *data);
		std::thread *thread;
		World *world;

	public:
		ChunkUpdater(World *world);
		~ChunkUpdater();
		void start();
		void stop();

	};

}

#endif
