#include "NBTTagLong.h"
#include "NBTException.h"
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
		if (!file->readInt64(&this->value))
			throw NBTException("NBTTagLong: invalid read");
	}

	void NBTTagLong::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt64(this->value))
			throw NBTException("NBTTagLong: invalid write");
	}

	void NBTTagLong::printDebug()
	{
		LOG("NBTTag_Long(\"" << this->name << "\") : " << this->value);
	}

}
