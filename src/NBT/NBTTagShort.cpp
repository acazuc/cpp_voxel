#include "NBTTagShort.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagShort::NBTTagShort(std::string name)
	: NBTTag(NBT_TAG_SHORT, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagShort::readDataFromFile(NBTFile *file)
	{
		if (!file->readInt16(&this->value))
			throw NBTException("NBTTagShort: invalid read");
	}

	void NBTTagShort::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt16(this->value))
			throw NBTException("NBTTagShort: invalid write");
	}

	void NBTTagShort::printDebug()
	{
		LOG("NBTTag_Short(\"" << this->name << "\") : " << this->value);
	}

}
