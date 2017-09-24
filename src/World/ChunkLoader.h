#ifndef CHUNK_LOADER_H
# define CHUNK_LOADER_H

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

	class ChunkLoader
	{

	private:
		static bool running;
		static bool checkChunk(World &world, int32_t x, int32_t z);
		static void run(void *data);
		std::thread *thread;
		World *world;

	public:
		ChunkLoader(World *world);
		~ChunkLoader();
		void start();
		void stop();

	};

}

#endif
