#ifndef BLOCKS_H
# define BLOCKS_H

# include "./Block.h"

namespace voxel
{

	class Blocks
	{

	private:
		static Block **blocks;

	public:
		static void init();
		static inline Block *getBlock(uint8_t id) {return (blocks[id]);};

	};

}

#endif
