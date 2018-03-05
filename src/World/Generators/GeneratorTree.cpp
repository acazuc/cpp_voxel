#include "GeneratorTree.h"
#include "World/World.h"

namespace voxel
{

	void GeneratorTree::generate(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z)
	{
		chunk.setBlock(x, y - 1, z, 3);
		for (int8_t y2 = 3; y2 <= 6; ++y2)
		{
			int8_t x2min = -2;
			int8_t x2max = 2;
			int8_t z2min = -2;
			int8_t z2max = 2;
			if (y2 >= 5)
			{
				++x2min;
				--x2max;
				++z2min;
				--z2max;
			}
			for (int8_t x2 = x2min; x2 <= x2max; ++x2)
			{
				for (int8_t z2 = z2min; z2 <= z2max; ++z2)
				{
					if (x2 == 0 && z2 == 0 && y2 <= 5)
						continue;
					if (y2 == 6 && x2 && (x2 == z2 || x2 == -z2))
						continue;
					chunk.getWorld().setBlockIfReplaceable(chunk.getX() + x + x2, y + y2, chunk.getZ() + z + z2, 18);
				}
			}
		}
		for (int32_t y2 = y; y2 < y + 6; ++y2)
			chunk.setBlockIfReplaceable(x, y2, z, 17);
	}

}
