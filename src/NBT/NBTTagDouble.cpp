#include "NBTTagDouble.h"
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
		file->readDouble(&this->value);
		//LOG("Tag_Double(\"" << this->name << "\") : " << this->value);
	}

	void NBTTagDouble::writeDataToFile(NBTFile *file)
	{
		file->writeDouble(this->value);
	}

}
