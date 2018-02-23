#include "NBTTagLongArray.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagLongArray::NBTTagLongArray(std::string name)
	: NBTTag(NBT_TAG_LONG_ARRAY, name)
	{
		//Empty
	}

	void NBTTagLongArray::readData(NBTStream *stream)
	{
		int32_t size;
		if (!stream->readInt32(&size))
			throw NBTException("NBTTagLongArray: invalid read size");
		int64_t value;
		for (int32_t i = 0; i < size; ++i)
		{
			if (!stream->readInt64(&value))
				throw NBTException("NBTTagLongArray: invalid read value");
			this->values.push_back(value);
		}
	}

	void NBTTagLongArray::writeData(NBTStream *stream)
	{
		if (!stream->writeInt32(this->values.size()))
			throw NBTException("NBTTagLongArray: invalid write size");
		for (uint32_t i = 0; i < this->values.size(); ++i)
		{
			if (!stream->writeInt64(this->values[i]))
				throw NBTException("NBTTabLongArray: invalid write value");
		}
	}

	size_t NBTTagLongArray::getDataSize()
	{
		return (4 + this->values.size() * 8);
	}

	void NBTTagLongArray::printDebug()
	{
		LOG("NBTTag_LongArray(\"" << this->name << "\") : [" << this->values.size() << " longs]");
	}

}
