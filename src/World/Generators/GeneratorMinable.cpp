#include "GeneratorMinable.h"
#include "World/World.h"

namespace voxel
{

	GeneratorMinable::GeneratorMinable()
	: block(0)
	, size(0)
	{
		//Empty
	}

	void GeneratorMinable::generate(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z)
	{
		for (int8_t x2 = 0; x2 < this->size; ++x2)
		{
			for (int8_t y2 = 0; y2 < this->size; ++y2)
			{
				for (int8_t z2 = 0; z2 < this->size; ++z2)
				{
					chunk.getWorld().setBlock(chunk.getX() + x + x2, y + y2, chunk.getZ() + z + z2, this->block);
				}
			}
		}
	}

}
