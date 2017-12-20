#include "NBTTagFloat.h"
#include "NBTException.h"
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
		if (!file->readFloat(&this->value))
			throw NBTException("NBTTagFloat: invalid read");
	}

	void NBTTagFloat::writeDataToFile(NBTFile *file)
	{
		if (!file->writeFloat(this->value))
			throw NBTException("NBTTabFloat: invalid write");
	}

	void NBTTagFloat::printDebug()
	{
		LOG("NBTTag_Float(\"" << this->name << "\") : " << this->value);
	}

}
