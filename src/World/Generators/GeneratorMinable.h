#ifndef GENERATOR_MINABLE_H
# define GENERATOR_MINABLE_H

# include "Generator.h"

namespace voxel
{

	class GeneratorMinable
	{

	private:
		uint8_t block;
		uint8_t size;

	public:
		GeneratorMinable();
		void generate(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z);
		inline void setBlock(uint8_t block) {this->block = block;};
		inline void setSize(uint8_t size) {this->size = size;};

	};

}

#endif
