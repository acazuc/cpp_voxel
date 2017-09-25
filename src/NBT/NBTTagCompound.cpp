#include "NBTTagCompound.h"
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
		NBTTag *tag = NULL;
		while ((tag = file->readNextTag()))
		{
			tag->readDataFromFile(file);
			if (tag->getType() == NBT_TAG_END)
				return;
			this->tags.push_back(tag);
		}
		//LOG("Tag_Compound(\"" << this->name << "\") : " << this->tags.size() << " entries");
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
		file->writeInt8(0);
	}

}
