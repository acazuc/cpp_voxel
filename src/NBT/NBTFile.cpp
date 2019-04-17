#include "NBTFile.h"
#include "NBTException.h"
#include "NBTTagType.h"
#include "Debug.h"
#include <fstream>

namespace voxel
{

	NBTFile::NBTFile(std::string name)
	: name(name)
	{
	}

	void NBTFile::load()
	{
		if (this->istream.is_open())
			this->istream.close();
		this->istream.open(this->name, std::ios_base::in | std::ios_base::binary);
		if (!this->istream.is_open())
			throw NBTException("Failed to open file " + this->name);
		NBTTag *tag;
		while ((tag = NBTTag::readTag(this)))
			this->tags.push_back(tag);
		this->istream.close();
	}

	void NBTFile::save()
	{
		if (this->ostream.is_open())
			this->ostream.close();
		this->ostream.open(this->name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if (!this->ostream.is_open())
			throw NBTException("Failed to open file " + this->name);
		for (size_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->writeData(this);
		this->ostream.close();
	}

	void NBTFile::printDebug()
	{
		for (size_t i = 0; i < this->tags.size(); ++i)
			this->tags[i]->printDebug();
	}

	bool NBTFile::read(void *data, size_t len)
	{
		this->istream.read(reinterpret_cast<char*>(data), len);
		return this->istream.fail();
	}

	bool NBTFile::write(void *data, size_t len)
	{
		this->ostream.write(reinterpret_cast<char*>(data), len);
		return this->ostream.fail();
	}

}
