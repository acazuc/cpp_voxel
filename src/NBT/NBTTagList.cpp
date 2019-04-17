#include "NBTTagList.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagList::NBTTagList(std::string name)
	: NBTTag(NBT_TAG_LIST, name)
	{
	}

	NBTTagList::~NBTTagList()
	{
		for (size_t i = 0; i < this->values.size(); ++i)
			delete (this->values[i]);
	}

	void NBTTagList::readData(NBTStream *stream)
	{
		int32_t size;
		int8_t type;
		if (!stream->readInt8(&type))
			throw NBTException("NBTTagList: invalid read type");
		this->type = static_cast<enum NBTTagType>(type);
		if (!stream->readInt32(&size))
			throw NBTException("NBTTagList: invalid read size");
		for (int32_t i = 0; i < size; ++i)
		{
			this->values.push_back(NBTTag::getTagOfType(this->type, ""));
			this->values.back()->readData(stream);
		}
	}

	void NBTTagList::writeData(NBTStream *stream)
	{
		if (!stream->writeInt8(this->type))
			throw NBTException("NBTTagList: invalid write type");
		if (!stream->writeInt32(this->values.size()))
			throw NBTException("NBTTagList: invalid write size");
		for (size_t i = 0; i < this->values.size(); ++i)
			this->values[i]->writeData(stream);
	}

	size_t NBTTagList::getDataSize()
	{
		size_t len = 1 + 4;
		for (size_t i = 0; i < this->values.size(); ++i)
			len += this->values[i]->getDataSize();
		return len;
	}

	void NBTTagList::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_List(\"" << this->name << "\") : " << this->values.size() << " entries of type " << this->type << std::endl;
	}

}
