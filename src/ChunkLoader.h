#ifndef CHUNK_LOADER_H
# define CHUNK_LOADER_H

# include <cstdint>
# include <thread>

namespace voxel
{

	class World;

	class ChunkLoader
	{

	private:
		static bool running;
		static bool checkChunk(World &world, int32_t x, int32_t z);
		std::thread *thread;

	public:
		ChunkLoader(World *world);
		~ChunkLoader();
		static void _run(void *data);

	};

}

#endif
