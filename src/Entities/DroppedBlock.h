#ifndef DROPPED_BLOCK_H
# define DROPPED_BLOCK_H

# include "Entity.h"

namespace voxel
{

	class DroppedBlock : public Entity
	{

	private:
		uint8_t type;

	public:
		DroppedBlock(World &world, uint8_t type);
		void draw();

	};

}

#endif
