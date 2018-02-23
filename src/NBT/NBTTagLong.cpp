#include "NBTTagLong.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagLong::NBTTagLong(std::string name)
	: NBTTag(NBT_TAG_LONG, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagLong::readData(NBTStream *stream)
	{
		if (!stream->readInt64(&this->value))
			throw NBTException("NBTTagLong: invalid read");
	}

	void NBTTagLong::writeData(NBTStream *stream)
	{
		if (!stream->writeInt64(this->value))
			throw NBTException("NBTTagLong: invalid write");
	}

	size_t NBTTagLong::getDataSize()
	{
		return (8);
	}

	void NBTTagLong::printDebug()
	{
		LOG("NBTTag_Long(\"" << this->name << "\") : " << this->value);
	}

}
