#include "NBTMgr.h"

namespace voxel
{

	void NBTMgr::childTagByteDefault(NBTTagCompound *parent, NBTTagByte **tag, std::string name, int8_t value)
	{
		if (*tag)
			return;
		*tag = new NBTTagByte(name);
		(*tag)->setValue(value);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagShortDefault(NBTTagCompound *parent, NBTTagShort **tag, std::string name, int16_t value)
	{
		if (*tag)
			return;
		*tag = new NBTTagShort(name);
		(*tag)->setValue(value);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagIntDefault(NBTTagCompound *parent, NBTTagInt **tag, std::string name, int32_t value)
	{
		if (*tag)
			return;
		*tag = new NBTTagInt(name);
		(*tag)->setValue(value);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagLongDefault(NBTTagCompound *parent, NBTTagLong **tag, std::string name, int64_t value)
	{
		if (*tag)
			return;
		*tag = new NBTTagLong(name);
		(*tag)->setValue(value);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagFloatDefault(NBTTagCompound *parent, NBTTagFloat **tag, std::string name, float value)
	{
		if (*tag)
			return;
		*tag = new NBTTagFloat(name);
		(*tag)->setValue(value);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagDoubleDefault(NBTTagCompound *parent, NBTTagDouble **tag, std::string name, double value)
	{
		if (*tag)
			return;
		*tag = new NBTTagDouble(name);
		(*tag)->setValue(value);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagByteArrayDefault(NBTTagCompound *parent, NBTTagByteArray **tag, std::string name, int32_t len)
	{
		if (*tag)
			return;
		*tag = new NBTTagByteArray(name);
		(*tag)->getValues().resize(len, 0);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagStringDefault(NBTTagCompound *parent, NBTTagString **tag, std::string name, std::string value)
	{
		if (*tag)
			return;
		*tag = new NBTTagString(name);
		(*tag)->setValue(value);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagListDefault(NBTTagCompound *parent, NBTTagList **tag, std::string name, enum NBTTagType type)
	{
		if (*tag)
			return;
		*tag = new NBTTagList(name);
		(*tag)->setType(type);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagCompoundDefault(NBTTagCompound *parent, NBTTagCompound **tag, std::string name)
	{
		if (*tag)
			return;
		*tag = new NBTTagCompound(name);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagIntArrayDefault(NBTTagCompound *parent, NBTTagIntArray **tag, std::string name, int32_t len)
	{
		if (*tag)
			return;
		*tag = new NBTTagIntArray(name);
		(*tag)->getValues().resize(len, 0);
		parent->addTag(*tag);
	}

	void NBTMgr::childTagLongArrayDefault(NBTTagCompound *parent, NBTTagLongArray **tag, std::string name, int32_t len)
	{
		if (*tag)
			return;
		*tag = new NBTTagLongArray(name);
		(*tag)->getValues().resize(len, 0);
		parent->addTag(*tag);
	}

}
