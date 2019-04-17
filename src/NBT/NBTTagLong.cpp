#include "NBTTagLong.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagLong::NBTTagLong(std::string name)
	: NBTTag(NBT_TAG_LONG, name)
	, value(0)
	{
	}

	void NBTTagLong::readData(NBTStream *stream)
	{
		if (!stream->readInt64(&this->value))
			throw NBTException("NBTTagLong: invalid read");
	}

	void NBTTagLong::writeData(NBTStream *stream)
	{
		if (!stream->writeInt64(this->value))
			throw NBTException("NBTTagLong: invalid write");
	}

	size_t NBTTagLong::getDataSize()
	{
		return 8;
	}

	void NBTTagLong::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_Long(\"" << this->name << "\") : " << this->value << std::endl;
	}

}
