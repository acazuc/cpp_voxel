#include "NBTTagByte.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagByte::NBTTagByte(std::string name)
	: NBTTag(NBT_TAG_BYTE, name)
	, value(0)
	{
	}

	void NBTTagByte::readData(NBTStream *stream)
	{
		if (!stream->readInt8(&this->value))
			throw NBTException("NBTTagByte: invalid read");
	}

	void NBTTagByte::writeData(NBTStream *stream)
	{
		if (!stream->writeInt8(this->value))
			throw NBTException("NBTTagByte: invalid write");
	}

	size_t NBTTagByte::getDataSize()
	{
		return 1;
	}

	void NBTTagByte::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_Byte(\"" << this->name << "\") : " << (int)this->value << std::endl;
	}

}
