#include "NBTTagFloat.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagFloat::NBTTagFloat(std::string name)
	: NBTTag(NBT_TAG_FLOAT, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagFloat::readData(NBTStream *stream)
	{
		if (!stream->readFloat(&this->value))
			throw NBTException("NBTTagFloat: invalid read");
	}

	void NBTTagFloat::writeData(NBTStream *stream)
	{
		if (!stream->writeFloat(this->value))
			throw NBTException("NBTTabFloat: invalid write");
	}

	size_t NBTTagFloat::getDataSize()
	{
		return (4);
	}

	void NBTTagFloat::printDebug()
	{
		LOG("NBTTag_Float(\"" << this->name << "\") : " << this->value);
	}

}
