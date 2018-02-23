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

	void NBTTagList::readData(NBTStream *stream)
	{
		int32_t size;
		if (!stream->readInt8(reinterpret_cast<int8_t*>(&this->type)))
			throw NBTException("NBTTagList: invalid read type");
		if (!stream->readInt32(&size))
			throw NBTException("NBTTagList: invalid read size");
		for (int32_t i = 0; i < size; ++i)
		{
			this->values.push_back(NBTTag::readTagOfType(static_cast<enum NBTTagType>(this->type), ""));
			this->values.back()->readData(stream);
		}
	}

	void NBTTagList::writeData(NBTStream *stream)
	{
		if (!stream->writeInt8(this->type))
			throw NBTException("NBTTagList: invalid write type");
		if (!stream->writeInt32(this->values.size()))
			throw NBTException("NBTTagList: invalid write size");
		for (uint32_t i = 0; i < this->values.size(); ++i)
			this->values[i]->writeData(stream);
	}

	size_t NBTTagList::getDataSize()
	{
		size_t len = 1 + 4;
		for (uint32_t i = 0; i < this->values.size(); ++i)
			len += this->values[i]->getHeaderSize() + this->values[i]->getDataSize();
		return (len);
	}

	void NBTTagList::printDebug()
	{
		LOG("NBTTag_List(\"" << this->name << "\") : " << this->values.size() << " entries of type " << this->type);
	}

}
