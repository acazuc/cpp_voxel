#include "NBTTagCompound.h"
#include "NBTException.h"
#include <iostream>

namespace voxel
{

	NBTTagCompound::NBTTagCompound(std::string name)
	: NBTTag(NBT_TAG_COMPOUND, name)
	{
	}

	NBTTagCompound::~NBTTagCompound()
	{
		for (size_t i = 0; i < this->tags.size(); ++i)
			delete (this->tags[i]);
	}

	void NBTTagCompound::readData(NBTStream *stream)
	{
		NBTTag *tag;
		while ((tag = NBTTag::readTag(stream)))
		{
			if (tag->getType() == NBT_TAG_END)
			{
				delete (tag);
				return;
			}
			this->tags.push_back(tag);
		}
	}

	void NBTTagCompound::writeData(NBTStream *stream)
	{
		for (size_t i = 0; i < this->tags.size(); ++i)
		{
			NBTTag *tag = this->tags[i];
			tag->writeId(stream);
			tag->writeName(stream);
			tag->writeData(stream);
		}
		if (!stream->writeInt8(0))
			throw NBTException("NBTTagCompound: invalid write NBTTagEnd");
	}

	size_t NBTTagCompound::getDataSize()
	{
		size_t len = 4; //NBTTagEnd
		for (size_t i = 0; i < this->tags.size(); ++i)
			len += this->tags[i]->getHeaderSize() + this->tags[i]->getDataSize();
		return len;
	}

	void NBTTagCompound::printDebug(size_t tab)
	{
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "NBTTag_Compound(\"" << this->name << "\") : " << this->tags.size() << " entries" << std::endl;
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "{" << std::endl;
		for (size_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->printDebug(tab + 1);
		for (size_t i = 0; i < tab; ++i)
			std::cout << "\t";
		std::cout << "}" << std::endl;
	}

}
