#ifndef NBT_TAG_TYPE_H
# define NBT_TAG_TYPE_H

namespace voxel
{

	enum NBTTagType
	{
	
		NBT_TAG_END =		0x0,
		NBT_TAG_BYTE =		0x1,
		NBT_TAG_SHORT =		0x2,
		NBT_TAG_INT =		0x3,
		NBT_TAG_LONG =		0x4,
		NBT_TAG_FLOAT =		0x5,
		NBT_TAG_DOUBLE =	0x6,
		NBT_TAG_BYTE_ARRAY =	0x7,
		NBT_TAG_STRING =	0x8,
		NBT_TAG_LIST =		0x9,
		NBT_TAG_COMPOUND =	0xA,
		NBT_TAG_INT_ARRAY =	0xB,
		NBT_TAG_LONG_ARRAY =	0xC

	};

}

#endif
