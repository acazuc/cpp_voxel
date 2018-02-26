#ifndef NBT_MGR_H
# define NBT_MGR_H

# include "NBT/NBT.h"
# include <cstdint>
# include <string>

namespace voxel
{

	class NBTMgr
	{

	public:
		static void childTagByteDefault(NBTTagCompound *parent, NBTTagByte **tag, std::string name, int8_t value);
		static void childTagShortDefault(NBTTagCompound *parent, NBTTagShort **tag, std::string name, int16_t value);
		static void childTagIntDefault(NBTTagCompound *parent, NBTTagInt **tag, std::string name, int32_t value);
		static void childTagLongDefault(NBTTagCompound *parent, NBTTagLong **tag, std::string name, int64_t value);
		static void childTagFloatDefault(NBTTagCompound *parent, NBTTagFloat **tag, std::string name, float value);
		static void childTagDoubleDefault(NBTTagCompound *parent, NBTTagDouble **tag, std::string name, double value);
		static void childTagByteArrayDefault(NBTTagCompound *parent, NBTTagByteArray **tag, std::string name, int32_t len);
		static void childTagStringDefault(NBTTagCompound *parent, NBTTagString **tag, std::string name, std::string value);
		static void childTagListDefault(NBTTagCompound *parent, NBTTagList **tag, std::string name, enum NBTTagType type);
		static void childTagCompoundDefault(NBTTagCompound *parent, NBTTagCompound **tag, std::string name);
		static void childTagIntArrayDefault(NBTTagCompound *parent, NBTTagIntArray **tag, std::string name, int32_t len);
		static void childTagLongArrayDefault(NBTTagCompound *parent, NBTTagLongArray **tag, std::string name, int32_t len);

	};

}

#endif
