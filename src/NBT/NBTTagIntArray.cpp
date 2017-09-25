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
		this->size = file->readTagInt(file->readTagName());
		this->size->readDataFromFile(file);
		//LOG("Tag_IntArray(\"" << this->name << "\") : [" << this->size << " ints]");
		for (int32_t i = 0; i < this->size->getValue(); ++i)
		{
			int32_t value;
			file->readInt32(&value);
			this->values.push_back(value);
		}
	}

	void NBTTagIntArray::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
