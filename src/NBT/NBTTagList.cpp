#include "NBTTagList.h"
#include "NBTException.h"
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
		if (!file->readInt8(reinterpret_cast<int8_t*>(&this->type)))
			throw NBTException("NBTTagList: invalid read type");
		if (!file->readInt32(&size))
			throw NBTException("NBTTagList: invalid read size");
		for (int32_t i = 0; i < size; ++i)
		{
			this->values.push_back(file->readTagOfType(static_cast<enum NBTTagType>(this->type), ""));
			this->values.back()->readDataFromFile(file);
		}
	}

	void NBTTagList::writeDataToFile(NBTFile *file)
	{
		if (!file->writeInt8(this->type))
			throw NBTException("NBTTagList: invalid write type");
		if (!file->writeInt32(this->values.size()))
			throw NBTException("NBTTagList: invalid write size");
		for (uint32_t i = 0; i < this->values.size(); ++i)
			this->values[i]->writeDataToFile(file);
	}

	void NBTTagList::printDebug()
	{
		LOG("NBTTag_List(\"" << this->name << "\") : " << this->values.size() << " entries of type " << this->type);
	}

}
