#include "NBTTagByteArray.h"
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
		this->size = file->readTagInt(file->readTagName());
		this->size->readDataFromFile(file);
		//LOG("Tag_ByteArray(\"" << this->name << "\") : [" << this->size << " bytes]");
		for (int32_t i = 0; i < this->size->getValue(); ++i)
		{
			int8_t value;
			file->readInt8(&value);
			this->values.push_back(value);
		}
	}

	void NBTTagByteArray::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
