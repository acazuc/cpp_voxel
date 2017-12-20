#include "NBTTagEnd.h"
#include "Debug.h"

namespace voxel
{

	NBTTagEnd::NBTTagEnd()
	: NBTTag(NBT_TAG_END, "")
	{
		//Empty
	}

	void NBTTagEnd::readDataFromFile(NBTFile *file)
	{
		(void)file;
	}

	void NBTTagEnd::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

	void NBTTagEnd::printDebug()
	{
		LOG("NBTTag_End()");
	}

}
