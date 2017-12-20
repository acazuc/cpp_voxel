#include "NBTTag.h"
#include "NBTFile.h"

namespace voxel
{

	NBTTag::NBTTag(enum NBTTagType type, std::string name)
	: typeId(type)
	, name(name)
	{
		//Empty
	}

	NBTTag::~NBTTag()
	{
		//Empty
	}

	void NBTTag::writeIdToFile(NBTFile *file)
	{
		file->writeInt8(this->typeId);
	}

	void NBTTag::writeNameToFile(NBTFile *file)
	{
		file->writeInt16(this->name.length());
		if (this->name.length())
			file->writeData(const_cast<char*>(this->name.data()), this->name.length());
	}

	void NBTTag::printDebug()
	{
		//Empty
	}

}
