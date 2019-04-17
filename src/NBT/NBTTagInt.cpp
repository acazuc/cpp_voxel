#include "NBTTagInt.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagInt::NBTTagInt(std::string name)
	: NBTTag(NBT_TAG_INT, name)
	, value(0)
	{
	}

	void NBTTagInt::readData(NBTStream *stream)
	{
		if (!stream->readInt32(&this->value))
			throw NBTException("NBTTagInt: invalid read");
	}

	void NBTTagInt::writeData(NBTStream *stream)
	{
		if (!stream->writeInt32(this->value))
			throw NBTException("NBTTagInt: invalid write");
	}

	size_t NBTTagInt::getDataSize()
	{
		return 4;
	}

	void NBTTagInt::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_Int(\"" << this->name << "\") : " << this->value << std::endl;
	}

}
