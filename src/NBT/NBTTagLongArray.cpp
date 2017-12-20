#include "NBTTagLongArray.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagLongArray::NBTTagLongArray(std::string name)
	: NBTTag(NBT_TAG_LONG_ARRAY, name)
	{
		//Empty
	}

	void NBTTagLongArray::readDataFromFile(NBTFile *file)
	{
		int32_t size;
		if (!file->readInt32(&size))
			throw NBTException("NBTTagLongArray: invalid read size");
		int64_t value;
		for (int32_t i = 0; i < size; ++i)
		{
			if (!file->readInt64(&value))
				throw NBTException("NBTTagLongArray: invalid read value");
			this->values.push_back(value);
		}
	}

	void NBTTagLongArray::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt32(this->values.size()))
			throw NBTException("NBTTagLongArray: invalid write size");
		for (uint32_t i = 0; i < this->values.size(); ++i)
		{
			if (!file->writeInt64(this->values[i]))
				throw NBTException("NBTTabLongArray: invalid write value");
		}
	}

	void NBTTagLongArray::printDebug()
	{
		LOG("NBTTag_LongArray(\"" << this->name << "\") : [" << this->values.size() << " longs]");
	}

}
