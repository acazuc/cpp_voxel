#include "NBTTagByteArray.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagByteArray::NBTTagByteArray(std::string name)
	: NBTTag(NBT_TAG_BYTE_ARRAY, name)
	{
		//Empty
	}

	void NBTTagByteArray::readDataFromFile(NBTFile *file)
	{
		int32_t size;
		if (!file->readInt32(&size))
			throw NBTException("NBTTagByteArray: invalid read size");
		int8_t value;
		for (int32_t i = 0; i < size; ++i)
		{
			if (!file->readInt8(&value))
				throw NBTException("NBTTagByteArray: invalid read value");
			this->values.push_back(value);
		}
	}

	void NBTTagByteArray::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt32(this->values.size()))
			throw NBTException("NBTTagByteArray: invalid write size");
		for (uint32_t i = 0; i < this->values.size(); ++i)
		{
			if (!file->writeInt8(this->values[i]))
				throw NBTException("NBTTagByteArray: invalid write value");
		}
	}

	void NBTTagByteArray::printDebug()
	{
		LOG("NBTTag_ByteArray(\"" << this->name << "\") : [" << this->values.size() << " bytes]");
	}

}
