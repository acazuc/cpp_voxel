#include "NBTTag.h"
#include "NBTTagByteArray.h"
#include "NBTTagLongArray.h"
#include "NBTTagCompound.h"
#include "NBTTagIntArray.h"
#include "NBTException.h"
#include "NBTTagDouble.h"
#include "NBTTagString.h"
#include "NBTTagFloat.h"
#include "NBTTagShort.h"
#include "NBTTagByte.h"
#include "NBTTagList.h"
#include "NBTTagLong.h"
#include "NBTTagEnd.h"
#include "NBTTagInt.h"
#include "NBTFile.h"
#include <cstring>

namespace voxel
{

	NBTTag::NBTTag(enum NBTTagType type, std::string name)
	: typeId(type)
	, name(name)
	{
	}

	NBTTag::~NBTTag()
	{
	}

	void NBTTag::writeId(NBTStream *stream)
	{
		if (!stream->writeInt8(this->typeId))
			throw NBTException("Failed to write NBT id");
	}

	void NBTTag::writeName(NBTStream *stream)
	{
		stream->writeInt16(this->name.length());
		if (this->name.length())
			stream->write(const_cast<char*>(this->name.data()), this->name.length());
	}

	void NBTTag::writeHeader(NBTStream *stream)
	{
		writeId(stream);
		writeName(stream);
	}

	size_t NBTTag::getHeaderSize()
	{
		return 1 + 2 + this->name.length();
	}

	NBTTag *NBTTag::readTag(NBTStream *stream)
	{
		int8_t id;
		if (!stream->readInt8(&id))
			return nullptr;
		std::string name;
		if (id)
			name = readTagName(stream);
		NBTTag *tag = getTagOfType(static_cast<enum NBTTagType>(id), name);
		tag->readData(stream);
		return tag;
	}

	std::string NBTTag::readTagName(NBTStream *stream)
	{
		uint16_t length = 0;
		if (!stream->readInt16((int16_t*)&length))
			throw NBTException("Can't read name len");
		if (!length)
			return "";
		std::string result(length, '\0');
		if (!stream->read(const_cast<char*>(result.data()), (size_t)length))
			throw NBTException("Can't read name");
		return result;
	}

	NBTTag *NBTTag::getTagOfType(enum NBTTagType type, std::string name)
	{
		switch (type)
		{
			case NBT_TAG_END:
				return new NBTTagEnd();
			case NBT_TAG_BYTE:
				return new NBTTagByte(name);
			case NBT_TAG_SHORT:
				return new NBTTagShort(name);
			case NBT_TAG_INT:
				return new NBTTagInt(name);
			case NBT_TAG_LONG:
				return new NBTTagLong(name);
			case NBT_TAG_FLOAT:
				return new NBTTagFloat(name);
			case NBT_TAG_DOUBLE:
				return new NBTTagDouble(name);
			case NBT_TAG_BYTE_ARRAY:
				return new NBTTagByteArray(name);
			case NBT_TAG_STRING:
				return new NBTTagString(name);
			case NBT_TAG_LIST:
				return new NBTTagList(name);
			case NBT_TAG_COMPOUND:
				return new NBTTagCompound(name);
			case NBT_TAG_INT_ARRAY:
				return new NBTTagIntArray(name);
			case NBT_TAG_LONG_ARRAY:
				return new NBTTagLongArray(name);
			default:
				throw NBTException("Invalid NBTTag ID (" + std::to_string((int)type) + ")");
		}
	}

}
