#include "NBTTagString.h"
#include "NBTException.h"
#include "NBTFile.h"
#include "Debug.h"

namespace voxel
{

	NBTTagString::NBTTagString(std::string name)
	: NBTTag(NBT_TAG_STRING, name)
	{
		//Empty
	}

	void NBTTagString::readData(NBTStream *stream)
	{
		int16_t len = 0;
		if (!stream->readInt16(&len))
			throw NBTException("NBTTagString: invalid read length");
		this->value.resize(len + 1, '\0');
		if (len)
		{
			if (!stream->read(const_cast<char*>(this->value.data()), len))
				throw NBTException("NBTTagString: invalid read value");
		}
	}

	void NBTTagString::writeData(NBTStream *stream)
	{
		if (!stream->writeInt16(this->value.size()))
			throw NBTException("NBTTagString: invalid write length");
		if (this->value.size())
		{
			if (!stream->write(const_cast<char*>(this->value.data()), this->value.size()))
				throw NBTException("NBTTagString: invalid write value");
		}
	}

	size_t NBTTagString::getDataSize()
	{
		return (2 + this->value.size());
	}

	void NBTTagString::printDebug()
	{
		LOG("NBTTag_String(\"" << this->name << "\") : " << this->value);
	}

}
