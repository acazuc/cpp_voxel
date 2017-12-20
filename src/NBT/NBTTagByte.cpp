#include "NBTTagByte.h"
#include "NBTException.h"
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
		if (!file->readInt8(&this->value))
			throw NBTException("NBTTagByte: invalid read");
	}

	void NBTTagByte::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt8(this->value))
			throw NBTException("NBTTagByte: invalid write");
	}

	void NBTTagByte::printDebug()
	{
		LOG("NBTTag_Byte(\"" << this->name << "\") : " << (int)this->value);
	}

}
