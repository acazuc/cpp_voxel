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
			{
				return;
			}
			this->tags.push_back(tag);
		}
		//LOG("Tag_Compound(\"" << this->name << "\") : " << this->tags.size() << " entries");
	}

	void NBTTagCompound::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
