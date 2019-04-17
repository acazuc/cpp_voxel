#include "NBTTagFloat.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagFloat::NBTTagFloat(std::string name)
	: NBTTag(NBT_TAG_FLOAT, name)
	, value(0)
	{
	}

	void NBTTagFloat::readData(NBTStream *stream)
	{
		if (!stream->readFloat(&this->value))
			throw NBTException("NBTTagFloat: invalid read");
	}

	void NBTTagFloat::writeData(NBTStream *stream)
	{
		if (!stream->writeFloat(this->value))
			throw NBTException("NBTTabFloat: invalid write");
	}

	size_t NBTTagFloat::getDataSize()
	{
		return 4;
	}

	void NBTTagFloat::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_Float(\"" << this->name << "\") : " << this->value << std::endl;
	}

}
