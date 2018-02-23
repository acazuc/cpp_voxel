#include "NBTFile.h"
#include "NBTException.h"
#include "NBTTagType.h"
#include "Debug.h"

namespace voxel
{

	NBTFile::NBTFile(std::string name)
	: name(name)
	{
		//Empty
	}

	void NBTFile::load()
	{
		if (!this->istream.open(this->name))
			throw NBTException("Failed to open file " + this->name);
		NBTTag *tag;
		while ((tag = NBTTag::readTag(this)))
		{
			tag->readData(this);
			this->tags.push_back(tag);
		}
		this->istream.close();
	}

	void NBTFile::save()
	{
		if (!this->ostream.open(this->name))
			throw NBTException("Failed to open file " + this->name);
		for (uint32_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->writeData(this);
		this->ostream.close();
	}

	void NBTFile::printDebug()
	{
		for (uint32_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->printDebug();
	}

	bool NBTFile::read(void *data, size_t len)
	{
		return (this->istream.read(data, len) == (ssize_t)len);
	}

	bool NBTFile::write(void *data, size_t len)
	{
		return (this->ostream.write(data, len) == (ssize_t)len);
	}

}
