#ifndef GENERATOR_TREE_H
# define GENERATOR_TREE_H

# include <cstdint>

namespace voxel
{

	class World;
	class Chunk;

	class GeneratorTree
	{

	private:

	public:
		static void generate(World &world, Chunk &chunk, uint8_t x, uint8_t y, uint8_t z);

	};

}

#endif
