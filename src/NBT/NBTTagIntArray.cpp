#include "NBTTagIntArray.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagIntArray::NBTTagIntArray(std::string name)
	: NBTTag(NBT_TAG_INT_ARRAY, name)
	{
		//Empty
	}

	void NBTTagIntArray::readData(NBTStream *stream)
	{
		int32_t size;
		if (!stream->readInt32(&size))
			throw NBTException("NBTTagIntArray: invalid read size");
		int32_t value;
		for (int32_t i = 0; i < size; ++i)
		{
			if (!stream->readInt32(&value))
				throw NBTException("NBTTagIntArray: invalid read value");
			this->values.push_back(value);
		}
	}

	void NBTTagIntArray::writeData(NBTStream *stream)
	{
		if (!stream->writeInt32(this->values.size()))
			throw NBTException("NBTTagIntArray: invalid write size");
		for (uint32_t i = 0; i < this->values.size(); ++i)
		{
			if (!stream->writeInt32(this->values[i]))
				throw NBTException("NBTTagIntArray: inalid write value");
		}
	}

	size_t NBTTagIntArray::getDataSize()
	{
		return (4 + 4 * this->values.size());
	}

	void NBTTagIntArray::printDebug()
	{
		LOG("NBTTag_IntArray(\"" << this->name << "\") : [" << this->values.size() << " ints]");
	}

}
