#include "NBTTagFloat.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagFloat::NBTTagFloat(std::string name)
	: NBTTag(NBT_TAG_FLOAT, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagFloat::readDataFromFile(NBTFile *file)
	{
		file->readFloat(&this->value);
		//LOG("Tag_Float(\"" << this->name << "\") : " << this->value);
	}

	void NBTTagFloat::writeDataToFile(NBTFile *file)
	{
		file->writeFloat(this->value);
	}

}
