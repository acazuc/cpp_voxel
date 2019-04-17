#include "NBTTagEnd.h"
#include <iostream>

namespace voxel
{

	NBTTagEnd::NBTTagEnd()
	: NBTTag(NBT_TAG_END, "")
	{
	}

	void NBTTagEnd::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_End()" << std::endl;
	}

}
