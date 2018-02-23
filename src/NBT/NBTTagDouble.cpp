#include "NBTTagDouble.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagDouble::NBTTagDouble(std::string name)
	: NBTTag(NBT_TAG_DOUBLE, name)
	, value(0)
	{
		//Empty
	}

	void NBTTagDouble::readData(NBTStream *stream)
	{
		if (!stream->readDouble(&this->value))
			throw NBTException("NBTTagDouble: invalid read");
	}

	void NBTTagDouble::writeData(NBTStream *stream)
	{
		if (!stream->writeDouble(this->value))
			throw NBTException("NBTTagDouble: invalid write");
	}

	size_t NBTTagDouble::getDataSize()
	{
		return (8);
	}

	void NBTTagDouble::printDebug()
	{
		LOG("NBTTag_Double(\"" << this->name << "\") : " << this->value);
	}

}
