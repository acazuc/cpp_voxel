#include "NBTGZipFile.h"
#include "NBTException.h"
#include "NBTTagType.h"
#include "Debug.h"

namespace voxel
{

	NBTGZipFile::NBTGZipFile(std::string name)
	: name(name)
	{
	}

	void NBTGZipFile::load()
	{
		if (!this->istream.open(this->name))
			throw NBTException("Failed to open file " + this->name);
		NBTTag *tag;
		while ((tag = NBTTag::readTag(this)))
			this->tags.push_back(tag);
		this->istream.close();
	}

	void NBTGZipFile::save()
	{
		if (!this->ostream.open(this->name))
			throw NBTException("Failed to open file " + this->name);
		for (size_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->writeData(this);
		this->ostream.close();
	}

	void NBTGZipFile::printDebug()
	{
		for (size_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->printDebug();
	}

	bool NBTGZipFile::read(void *data, size_t len)
	{
		return this->istream.read(data, len) == (ssize_t)len;
	}

	bool NBTGZipFile::write(void *data, size_t len)
	{
		return this->ostream.write(data, len) == (ssize_t)len;
	}

}
