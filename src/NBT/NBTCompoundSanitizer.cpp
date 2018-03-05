#include "NBTCompoundSanitizer.h"
#include "NBT/NBTMgr.h"
#include "Debug.h"

namespace voxel
{

	NBTCompoundSanitizer::NBTCompoundSanitizer(NBTTagCompound *tag)
	: tag(tag)
	{
		//Empty
	}

	NBTCompoundSanitizer::~NBTCompoundSanitizer()
	{
		for (std::unordered_map<std::string, NBTCompoundSanitizerEntry*>::iterator iter = this->entries.begin(); iter != this->entries.end(); ++iter)
			delete (iter->second);
	}

	void NBTCompoundSanitizer::sanitize()
	{
		for (uint32_t i = 0; i < this->tag->getTags().size(); ++i)
		{
			NBTTag *tag = this->tag->getTags()[i];
			std::unordered_map<std::string, NBTCompoundSanitizerEntry*>::iterator iter = this->entries.find(tag->getName());
			if (iter == this->entries.end() || tag->getType() != iter->second->type)
				goto erase;
			if (iter->second->entry)
				*iter->second->entry = tag;
			continue;
erase:
			delete (this->tag->getTags()[i]);
			this->tag->getTags().erase(this->tag->getTags().begin() + i);
			i--;
		}
		for (std::unordered_map<std::string, NBTCompoundSanitizerEntry*>::iterator iter = this->entries.begin(); iter != this->entries.end(); ++iter)
		{
			NBTCompoundSanitizerEntry *entry = iter->second;
			switch (entry->type)
			{
				case NBT_TAG_BYTE:
					NBTMgr::childTagByteDefault(this->tag, reinterpret_cast<NBTTagByte**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryByte*>(entry)->value);
					break;
				case NBT_TAG_SHORT:
					NBTMgr::childTagShortDefault(this->tag, reinterpret_cast<NBTTagShort**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryShort*>(entry)->value);
					break;
				case NBT_TAG_INT:
					NBTMgr::childTagIntDefault(this->tag, reinterpret_cast<NBTTagInt**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryInt*>(entry)->value);
					break;
				case NBT_TAG_LONG:
					NBTMgr::childTagLongDefault(this->tag, reinterpret_cast<NBTTagLong**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryLong*>(entry)->value);
					break;
				case NBT_TAG_FLOAT:
					NBTMgr::childTagFloatDefault(this->tag, reinterpret_cast<NBTTagFloat**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryFloat*>(entry)->value);
					break;
				case NBT_TAG_DOUBLE:
					NBTMgr::childTagDoubleDefault(this->tag, reinterpret_cast<NBTTagDouble**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryDouble*>(entry)->value);
					break;
				case NBT_TAG_BYTE_ARRAY:
					NBTMgr::childTagByteArrayDefault(this->tag, reinterpret_cast<NBTTagByteArray**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryByteArray*>(entry)->len);
					break;
				case NBT_TAG_STRING:
					NBTMgr::childTagStringDefault(this->tag, reinterpret_cast<NBTTagString**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryString*>(entry)->value);
					break;
				case NBT_TAG_LIST:
					NBTMgr::childTagListDefault(this->tag, reinterpret_cast<NBTTagList**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryList*>(entry)->type);
					break;
				case NBT_TAG_COMPOUND:
					NBTMgr::childTagCompoundDefault(this->tag, reinterpret_cast<NBTTagCompound**>(entry->entry), entry->name);
					break;
				case NBT_TAG_INT_ARRAY:
					NBTMgr::childTagIntArrayDefault(this->tag, reinterpret_cast<NBTTagIntArray**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryIntArray*>(entry)->len);
					break;
				case NBT_TAG_LONG_ARRAY:
					NBTMgr::childTagLongArrayDefault(this->tag, reinterpret_cast<NBTTagLongArray**>(entry->entry), entry->name, reinterpret_cast<NBTCompoundSanitizerEntryLongArray*>(entry)->len);
					break;
				case NBT_TAG_END:
					break;
			}
		}
	}

}
