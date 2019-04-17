#include "NBTTagDouble.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagDouble::NBTTagDouble(std::string name)
	: NBTTag(NBT_TAG_DOUBLE, name)
	, value(0)
	{
	}

	void NBTTagDouble::readData(NBTStream *stream)
	{
		if (!stream->readDouble(&this->value))
			throw NBTException("NBTTagDouble: invalid read");
	}

	void NBTTagDouble::writeData(NBTStream *stream)
	{
		if (!stream->writeDouble(this->value))
			throw NBTException("NBTTagDouble: invalid write");
	}

	size_t NBTTagDouble::getDataSize()
	{
		return 8;
	}

	void NBTTagDouble::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_Double(\"" << this->name << "\") : " << this->value << std::endl;
	}

}
