#include "NBTTagShort.h"
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
		file->readInt16(&this->value);
		//LOG("Tag_Short(\"" << this->name << "\") : " << this->value);
	}

	void NBTTagShort::writeDataToFile(NBTFile *file)
	{
		file->writeInt16(this->value);
	}

}
