#ifndef ITEMS_H
# define ITEMS_H

# include "./Item.h"

namespace voxel
{

	class Items
	{

	private:
		Item **items;

	public:
		static void init();

	};

}

#endif
