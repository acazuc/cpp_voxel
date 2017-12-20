#include "NBTTagInt.h"
#include "NBTException.h"
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
		if (!file->readInt32(&this->value))
			throw NBTException("NBTTagInt: invalid read");
	}

	void NBTTagInt::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt32(this->value))
			throw NBTException("NBTTagInt: invalid write");
	}

	void NBTTagInt::printDebug()
	{
		LOG("NBTTag_Int(\"" << this->name << "\") : " << this->value);
	}

}
