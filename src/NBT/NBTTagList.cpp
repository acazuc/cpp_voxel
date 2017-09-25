#include "NBTTagList.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagList::NBTTagList(std::string name)
	: NBTTag(NBT_TAG_LIST, name)
	{
		//Empty
	}

	void NBTTagList::readDataFromFile(NBTFile *file)
	{
		file->readInt8(reinterpret_cast<int8_t*>(&this->type));
		file->readInt32(&this->size);
		//LOG("Tag_List(\"" << this->name << "\") : " << this->size << " entries of type " << this->type);
		for (int32_t i = 0; i < this->size; ++i)
		{
			this->values.push_back(file->readTagOfType(static_cast<enum NBTTagType>(this->type), ""));
			this->values.back()->readDataFromFile(file);
		}
	}

	void NBTTagList::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
