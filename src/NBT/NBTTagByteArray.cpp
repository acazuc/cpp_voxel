#include "NBTTagByteArray.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagByteArray::NBTTagByteArray(std::string name)
	: NBTTag(NBT_TAG_BYTE_ARRAY, name)
	{
	}

	void NBTTagByteArray::readData(NBTStream *stream)
	{
		int32_t size;
		if (!stream->readInt32(&size))
			throw NBTException("NBTTagByteArray: invalid read size");
		this->values.resize(size);
		if (!stream->read(this->values.data(), size))
			throw NBTException("NBTTagByteArray: invalid read value");
	}

	void NBTTagByteArray::writeData(NBTStream *stream)
	{
		if (!stream->writeInt32(this->values.size()))
			throw NBTException("NBTTagByteArray: invalid write size");
		if (!stream->write(this->values.data(), this->values.size()))
			throw NBTException("NBTTagByteArray: invalid write value");
	}

	size_t NBTTagByteArray::getDataSize()
	{
		return 4 + this->values.size();
	}

	void NBTTagByteArray::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_ByteArray(\"" << this->name << "\") : [" << this->values.size() << " bytes]" << std::endl;
	}

}
