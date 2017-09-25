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
		int32_t size;
		file->readInt8(reinterpret_cast<int8_t*>(&this->type));
		file->readInt32(&size);
		//LOG("Tag_List(\"" << this->name << "\") : " << size << " entries of type " << this->type);
		for (int32_t i = 0; i < size; ++i)
		{
			this->values.push_back(file->readTagOfType(static_cast<enum NBTTagType>(this->type), ""));
			this->values.back()->readDataFromFile(file);
		}
	}

	void NBTTagList::writeDataToFile(NBTFile *file)
	{
		file->writeInt8(this->type);
		file->writeInt32(this->values.size());
		for (uint32_t i = 0; i < this->values.size(); ++i)
			this->values[i]->writeDataToFile(file);
	}

}
