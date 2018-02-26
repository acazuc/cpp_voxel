#include "NBTMgr.h"

namespace voxel
{

	void NBTMgr::childTagByteDefault(NBTTagCompound *parent, NBTTagByte **tag, std::string name, int8_t value)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagByte *val = new NBTTagByte(name);
		val->setValue(value);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagShortDefault(NBTTagCompound *parent, NBTTagShort **tag, std::string name, int16_t value)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagShort *val = new NBTTagShort(name);
		val->setValue(value);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagIntDefault(NBTTagCompound *parent, NBTTagInt **tag, std::string name, int32_t value)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagInt *val = new NBTTagInt(name);
		val->setValue(value);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagLongDefault(NBTTagCompound *parent, NBTTagLong **tag, std::string name, int64_t value)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagLong *val = new NBTTagLong(name);
		val->setValue(value);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagFloatDefault(NBTTagCompound *parent, NBTTagFloat **tag, std::string name, float value)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagFloat *val = new NBTTagFloat(name);
		val->setValue(value);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagDoubleDefault(NBTTagCompound *parent, NBTTagDouble **tag, std::string name, double value)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagDouble *val = new NBTTagDouble(name);
		val->setValue(value);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagByteArrayDefault(NBTTagCompound *parent, NBTTagByteArray **tag, std::string name, int32_t len)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagByteArray *val = new NBTTagByteArray(name);
		val->getValues().resize(len, 0);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagStringDefault(NBTTagCompound *parent, NBTTagString **tag, std::string name, std::string value)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagString *val = new NBTTagString(name);
		val->setValue(value);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagListDefault(NBTTagCompound *parent, NBTTagList **tag, std::string name, enum NBTTagType type)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagList *val = new NBTTagList(name);
		val->setType(type);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagCompoundDefault(NBTTagCompound *parent, NBTTagCompound **tag, std::string name)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagCompound *val = new NBTTagCompound(name);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagIntArrayDefault(NBTTagCompound *parent, NBTTagIntArray **tag, std::string name, int32_t len)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagIntArray *val = new NBTTagIntArray(name);
		val->getValues().resize(len, 0);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

	void NBTMgr::childTagLongArrayDefault(NBTTagCompound *parent, NBTTagLongArray **tag, std::string name, int32_t len)
	{
		if (!tag && !parent)
			return;
		if (tag && *tag)
			return;
		NBTTagLongArray *val = new NBTTagLongArray(name);
		val->getValues().resize(len, 0);
		if (tag)
			(*tag) = val;
		if (parent)
			parent->addTag(val);
	}

}
