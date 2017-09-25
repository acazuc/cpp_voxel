#include "NBTTag.h"

namespace voxel
{

	NBTTag::NBTTag(enum NBTTagType type, std::string name)
	: typeId(type)
	, name(name)
	{
		//Empty
	}

}
