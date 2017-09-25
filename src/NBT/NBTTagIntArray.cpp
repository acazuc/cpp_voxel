#include "NBTTagIntArray.h"
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
		file->readInt32(&size);
		//LOG("Tag_IntArray(\"" << this->name << "\") : [" << size << " ints]");
		for (int32_t i = 0; i < size; ++i)
		{
			int32_t value;
			file->readInt32(&value);
			this->values.push_back(value);
		}
	}

	void NBTTagIntArray::writeDataToFile(NBTFile *file)
	{
		file->writeInt32(this->values.size());
		for (uint32_t i = 0; i < this->values.size(); ++i)
			file->writeInt32(this->values[i]);
	}

}
