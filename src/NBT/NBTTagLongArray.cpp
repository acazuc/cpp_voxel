#include "NBTTagLongArray.h"
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
		file->readInt32(&size);
		//LOG("Tag_LongArray(\"" << this->name << "\") : [" << size << " longs]");
		for (int32_t i = 0; i < size; ++i)
		{
			int64_t value;
			file->readInt64(&value);
			this->values.push_back(value);
		}
	}

	void NBTTagLongArray::writeDataToFile(NBTFile *file)
	{
		file->writeInt32(this->values.size());
		for (uint32_t i = 0; i < this->values.size(); ++i)
			file->writeInt64(this->values[i]);
	}

}
