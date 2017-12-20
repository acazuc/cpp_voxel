#include "NBTTagDouble.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagDouble::NBTTagDouble(std::string name)
	: NBTTag(NBT_TAG_DOUBLE, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagDouble::readDataFromFile(NBTFile *file)
	{
		if (!file->readDouble(&this->value))
			throw NBTException("NBTTagDouble: invalid read");
	}

	void NBTTagDouble::writeDataToFile(NBTFile *file)
	{
		if (!file->writeDouble(this->value))
			throw NBTException("NBTTagDouble: invalid write");
	}

	void NBTTagDouble::printDebug()
	{
		LOG("NBTTag_Double(\"" << this->name << "\") : " << this->value);
	}

}
