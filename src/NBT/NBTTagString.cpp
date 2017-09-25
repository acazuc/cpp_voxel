#include "NBTTagString.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagString::NBTTagString(std::string name)
	: NBTTag(NBT_TAG_SHORT, name)
	, value("")
	{
		//Empty
	}

	void NBTTagString::readDataFromFile(NBTFile *file)
	{
		int16_t len = 0;
		file->readInt16(&len);
		this->value.resize(len + 1, '\0');
		file->readData(const_cast<char*>(this->value.data()), len);
		//LOG("Tag_String(\"" << this->name << "\") : " << this->value);
	}

	void NBTTagString::writeDataToFile(NBTFile *file)
	{
		file->writeInt16(this->value.size());
		if (this->value.size())
			file->writeData(const_cast<char*>(this->value.data()), this->value.size());
	}

}
