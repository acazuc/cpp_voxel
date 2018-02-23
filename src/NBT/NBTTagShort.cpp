#include "NBTTagShort.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagShort::NBTTagShort(std::string name)
	: NBTTag(NBT_TAG_SHORT, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagShort::readData(NBTStream *stream)
	{
		if (!stream->readInt16(&this->value))
			throw NBTException("NBTTagShort: invalid read");
	}

	void NBTTagShort::writeData(NBTStream *stream)
	{
		if (!stream->writeInt16(this->value))
			throw NBTException("NBTTagShort: invalid write");
	}

	size_t NBTTagShort::getDataSize()
	{
		return (2);
	}

	void NBTTagShort::printDebug()
	{
		LOG("NBTTag_Short(\"" << this->name << "\") : " << this->value);
	}

}
