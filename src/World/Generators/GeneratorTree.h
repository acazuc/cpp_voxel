#ifndef GENERATOR_TREE_H
# define GENERATOR_TREE_H

# include "Generator.h"

namespace voxel
{

	class GeneratorTree : public Generator
	{

	public:
		void generate(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z);

	};

}

#endif
