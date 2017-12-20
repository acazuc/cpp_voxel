#include "NBTTagIntArray.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagIntArray::NBTTagIntArray(std::string name)
	: NBTTag(NBT_TAG_INT_ARRAY, name)
	{
		//Empty
	}

	void NBTTagIntArray::readDataFromFile(NBTFile *file)
	{
		int32_t size;
		if (!file->readInt32(&size))
			throw NBTException("NBTTagIntArray: invalid read size");
		int32_t value;
		for (int32_t i = 0; i < size; ++i)
		{
			if (!file->readInt32(&value))
				throw NBTException("NBTTagIntArray: invalid read value");
			this->values.push_back(value);
		}
	}

	void NBTTagIntArray::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt32(this->values.size()))
			throw NBTException("NBTTagIntArray: invalid write size");
		for (uint32_t i = 0; i < this->values.size(); ++i)
		{
			if (!file->writeInt32(this->values[i]))
				throw NBTException("NBTTagIntArray: inalid write value");
		}
	}

	void NBTTagIntArray::printDebug()
	{
		LOG("NBTTag_IntArray(\"" << this->name << "\") : [" << this->values.size() << " ints]");
	}

}
