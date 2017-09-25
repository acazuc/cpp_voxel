#include "NBTTagByte.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagByte::NBTTagByte(std::string name)
	: NBTTag(NBT_TAG_BYTE, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagByte::readDataFromFile(NBTFile *file)
	{
		file->readInt8(&this->value);
		//LOG("Tag_Byte(\"" << this->name << "\") : " << (int)this->value);
	}

	void NBTTagByte::writeDataToFile(NBTFile *file)
	{
		file->writeInt8(this->value);
	}

}
