#include "NBTTagLong.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagLong::NBTTagLong(std::string name)
	: NBTTag(NBT_TAG_LONG, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagLong::readDataFromFile(NBTFile *file)
	{
		file->readInt64(&this->value);
		//LOG("Tag_Long(\"" << this->name << "\") : " << this->value);
	}

	void NBTTagLong::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
