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
		this->size = file->readTagInt(file->readTagName());
		this->size->readDataFromFile(file);
		//LOG("Tag_LongArray(\"" << this->name << "\") : [" << this->size << " longs]");
		for (int32_t i = 0; i < this->size->getValue(); ++i)
		{
			int64_t value;
			file->readInt64(&value);
			this->values.push_back(value);
		}
	}

	void NBTTagLongArray::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
