#include "NBTCompoundSanitizer.h"

namespace voxel
{

	NBTCompoundSanitizer::NBTCompoundSanitizer(NBTTagCompound *tag)
	: tag(tag)
	{
		//Empty
	}

	void NBTCompoundSanitizer::sanitize()
	{
		for (uint32_t i = 0; i < this->tag->getTags().size(); ++i)
		{
			NBTTag *tag = this->tag->getTags()[i];
			std::unordered_map<std::string, NBTCompoundSanitizerEntry>::iterator iter = this->entries.find(tag->getName());
			if (iter == this->entries.end() || tag->getType() != iter->second.type)
				goto erase;
			if (iter->second.entry)
				*iter->second.entry = tag;
			continue;
erase:
			delete (this->tag->getTags()[i]);
			this->tag->getTags().erase(this->tag->getTags().begin() + i);
			i--;
		}
	}

}
