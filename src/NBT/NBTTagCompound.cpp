#include "NBTTagCompound.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"
#include <cstring>

namespace voxel
{

	NBTTagCompound::NBTTagCompound(std::string name)
	: NBTTag(NBT_TAG_COMPOUND, name)
	{
		//Empty
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
			tag->readData(stream);
			this->tags.push_back(tag);
		}
	}

	void NBTTagCompound::writeData(NBTStream *stream)
	{
		for (uint32_t i = 0; i < this->tags.size(); ++i)
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
		for (uint32_t i = 0; i < this->tags.size(); ++i)
			len += this->tags[i]->getHeaderSize() + this->tags[i]->getDataSize();
		return (len);
	}

	void NBTTagCompound::printDebug()
	{
		LOG("NBTTag_Compound(\"" << this->name << "\") : " << this->tags.size() << " entries");
		LOG("{");
		for (uint32_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->printDebug();
		LOG("}");
	}

}
