#include "NBTTagCompound.h"
#include "NBTException.h"
#include "Debug.h"
#include "NBTFile.h"

namespace voxel
{

	NBTTagCompound::NBTTagCompound(std::string name)
	: NBTTag(NBT_TAG_COMPOUND, name)
	{
		//Empty
	}

	void NBTTagCompound::readDataFromFile(NBTFile *file)
	{
		NBTTag *tag;
		while ((tag = file->readNextTag()))
		{
			if (tag->getType() == NBT_TAG_END)
			{
				delete (tag);
				return;
			}
			tag->readDataFromFile(file);
			this->tags.push_back(tag);
		}
	}

	void NBTTagCompound::writeDataToFile(NBTFile *file)
	{
		for (uint32_t i = 0; i < this->tags.size(); ++i)
		{
			NBTTag *tag = this->tags[i];
			tag->writeIdToFile(file);
			tag->writeNameToFile(file);
			tag->writeDataToFile(file);
		}
		if (!file->writeInt8(0))
			throw NBTException("NBTTagCompound: invalid write NBTTagEnd");
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
