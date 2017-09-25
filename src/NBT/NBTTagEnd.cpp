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
		//LOG("Tag_End()");
	}

	void NBTTagEnd::writeDataToFile(NBTFile *file)
	{
		(void)file;
	}

}
