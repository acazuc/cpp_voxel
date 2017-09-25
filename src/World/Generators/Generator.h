#ifndef GENERATOR_H
# define GENERATOR_H

# include <cstdint>

namespace voxel
{

	class Chunk;

	class Generator
	{

	public:
		void generate(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z);

	};

}

#endif
