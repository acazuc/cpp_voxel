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
		int32_t size;
		file->readInt32(&size);
		//LOG("Tag_ByteArray(\"" << this->name << "\") : [" << size << " bytes]");
		for (int32_t i = 0; i < size; ++i)
		{
			int8_t value;
			file->readInt8(&value);
			this->values.push_back(value);
		}
	}

	void NBTTagByteArray::writeDataToFile(NBTFile *file)
	{
		file->writeInt32(this->values.size());
		for (uint32_t i = 0; i < this->values.size(); ++i)
			file->writeInt8(this->values[i]);
	}

}
