#include "Region.h"
#include <cstring>

namespace voxel
{

	Region::Region(World &world)
	: world(world)
	{
		std::memset(this->chunks, 0, sizeof(this->chunks));
	}

	Region::~Region()
	{
		for (uint32_t x = 0; x < REGION_WIDTH; ++x)
		{
			for (uint32_t y = 0; y < REGION_WIDTH; ++y)
				delete (this->chunks[x][y]);
		}
	}

}
