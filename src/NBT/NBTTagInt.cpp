#include "NBTTagInt.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagInt::NBTTagInt(std::string name)
	: NBTTag(NBT_TAG_INT, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagInt::readDataFromFile(NBTFile *file)
	{
		file->readInt32(&this->value);
		//LOG("Tag_Int(\"" << this->name << "\") : " << this->value);
	}

	void NBTTagInt::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
