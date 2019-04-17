#ifndef PLAYER_INVENTORY_H
# define PLAYER_INVENTORY_H

# include "PlayerInventorySlot.h"

namespace voxel
{

	class PlayerInventory
	{

	private:
		PlayerInventorySlot slots[36];
		int8_t barPos;

	public:
		PlayerInventory();
		~PlayerInventory();
		PlayerInventorySlot *getSlots() {return this->slots;};
		inline int8_t &getBarPos() {return this->barPos;};

	};

}

#endif
