#include "NBTFile.h"
#include "NBTException.h"
#include "NBTTagType.h"
#include "Debug.h"
#include <libgzip/GZipInputStream.h>

using libgzip::GZipInputStream;

namespace voxel
{

	NBTFile::NBTFile(std::string name)
	: name(name)
	{
		if (!this->istream.open(this->name))
			throw NBTException("Failed to open file " + this->name);
	}

	void NBTFile::load()
	{
		NBTTag *tag = NULL;
		while ((tag = readNextTag()))
		{
			tag->readDataFromFile(this);
			this->tags.push_back(tag);
		}
	}

	std::string NBTFile::readTagName()
	{
		int16_t len = 0;
		if (!readInt16(&len))
			throw NBTException("Can't read name len");
		std::string result(len + 1, '\0');
		if (!readData(const_cast<char*>(result.data()), (size_t)len))
			throw NBTException("Can't read name");
		return (result);
	}

	bool NBTFile::readInt8(int8_t *data)
	{
		return (this->istream.readInt8(data));
	}

	bool NBTFile::readInt16(int16_t *data)
	{
		int16_t val;
		if (!this->istream.readInt16(&val))
			return (false);
		*(reinterpret_cast<uint16_t*>(data)) = htons(*reinterpret_cast<uint16_t*>(&val));
		return (true);
	}

	bool NBTFile::readInt32(int32_t *data)
	{
		int32_t val;
		if (!this->istream.readInt32(&val))
			return (false);
		*(reinterpret_cast<uint32_t*>(data)) = htonl(*reinterpret_cast<uint32_t*>(&val));
		return (true);
	}

	bool NBTFile::readInt64(int64_t *data)
	{
		int64_t val;
		if (!this->istream.readInt64(&val))
			return (false);
		*(reinterpret_cast<uint64_t*>(data)) = htonll(*reinterpret_cast<uint64_t*>(&val));
		return (true);
	}

	bool NBTFile::readFloat(float *data)
	{
		int32_t val;
		if (!this->istream.readInt32(&val))
			return (false);
		*(reinterpret_cast<uint32_t*>(data)) = htonl(*reinterpret_cast<uint32_t*>(&val));
		return (true);
	}

	bool NBTFile::readDouble(double *data)
	{
		int64_t val;
		if (!this->istream.readInt64(&val))
			return (false);
		*(reinterpret_cast<uint64_t*>(data)) = htonll(*reinterpret_cast<uint64_t*>(&val));
		return (true);
	}

	bool NBTFile::readData(void *data, size_t len)
	{
		return (this->istream.read(data, len));
	}

	uint16_t NBTFile::htons(uint16_t val)
	{
		int a = 42;
		if (*reinterpret_cast<char*>(&a) != 42)
			return (val);
		return (((val & 0xFF) << 8) | ((val & 0xFF00) >> 8));
	}

	uint32_t NBTFile::htonl(uint32_t val)
	{
		int a = 42;
		if (*reinterpret_cast<char*>(&a) != 42)
			return (val);
		return (((val & 0xFF) << 24) | ((val & 0xFF00) << 8) | ((val & 0xFF0000) >> 8) | ((val & 0xFF000000) >> 24));
	}

	uint64_t NBTFile::htonll(uint64_t val)
	{
		int a = 42;
		if (*reinterpret_cast<char*>(&a) != 42)
			return (val);
		return (((val & 0xFF) << 56) | ((val & 0xFF00) << 40) | ((val & 0xFF0000) << 24) | ((val & 0xFF000000) << 8)
				| ((val & 0xFF00000000) >> 8) | ((val & 0xFF0000000000) >> 24) | ((val & 0xFF000000000000) >> 40) | ((val & 0xFF00000000000000) >> 56));
	}

	NBTTag *NBTFile::readNextTag()
	{
		int8_t id;
		if (!readInt8(&id))
			return (NULL);
		std::string name;
		if (id)
			name = readTagName();
		NBTTag *tag = readTagOfType(static_cast<enum NBTTagType>(id), name);
		return (tag);
	}

	NBTTag *NBTFile::readTagOfType(enum NBTTagType type, std::string name)
	{
		switch (type)
		{
			case NBT_TAG_END:
				return (readTagEnd());
			case NBT_TAG_BYTE:
				return (readTagByte(name));
			case NBT_TAG_SHORT:
				return (readTagShort(name));
			case NBT_TAG_INT:
				return (readTagInt(name));
			case NBT_TAG_LONG:
				return (readTagLong(name));
			case NBT_TAG_FLOAT:
				return (readTagFloat(name));
			case NBT_TAG_DOUBLE:
				return (readTagDouble(name));
			case NBT_TAG_BYTE_ARRAY:
				return (readTagByteArray(name));
			case NBT_TAG_STRING:
				return (readTagString(name));
			case NBT_TAG_LIST:
				return (readTagList(name));
			case NBT_TAG_COMPOUND:
				return (readTagCompound(name));
			case NBT_TAG_INT_ARRAY:
				return (readTagIntArray(name));
			case NBT_TAG_LONG_ARRAY:
				return (readTagLongArray(name));
			default:
				throw NBTException("Invalid NBTTag ID (" + std::to_string((int)type) + ")");
		}
	}

	NBTTagEnd *NBTFile::readTagEnd()
	{
		return (new NBTTagEnd());
	}

	NBTTagByte *NBTFile::readTagByte(std::string name)
	{
		return (new NBTTagByte(name));
	}

	NBTTagShort *NBTFile::readTagShort(std::string name)
	{
		return (new NBTTagShort(name));
	}

	NBTTagInt *NBTFile::readTagInt(std::string name)
	{
		return (new NBTTagInt(name));
	}

	NBTTagLong *NBTFile::readTagLong(std::string name)
	{
		return (new NBTTagLong(name));
	}

	NBTTagFloat *NBTFile::readTagFloat(std::string name)
	{
		return (new NBTTagFloat(name));
	}

	NBTTagDouble *NBTFile::readTagDouble(std::string name)
	{
		return (new NBTTagDouble(name));
	}

	NBTTagByteArray *NBTFile::readTagByteArray(std::string name)
	{
		return (new NBTTagByteArray(name));
	}

	NBTTagString *NBTFile::readTagString(std::string name)
	{
		return (new NBTTagString(name));
	}

	NBTTagList *NBTFile::readTagList(std::string name)
	{
		return (new NBTTagList(name));
	}

	NBTTagCompound *NBTFile::readTagCompound(std::string name)
	{
		return (new NBTTagCompound(name));
	}

	NBTTagIntArray *NBTFile::readTagIntArray(std::string name)
	{
		return (new NBTTagIntArray(name));
	}

	NBTTagLongArray *NBTFile::readTagLongArray(std::string name)
	{
		return (new NBTTagLongArray(name));
	}

}
