#ifndef REGION_H
# define REGION_H

# include "./Chunk.h"

# define REGION_WIDTH 32

namespace voxel
{

	class World;

	class Region
	{

	private:
		Chunk *chunks[32][32];
		World &world;
		int32_t x;
		int32_t y;

	public:
		Region(World &world);
		~Region();

	};

}

#endif
