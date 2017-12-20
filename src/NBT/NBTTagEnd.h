#ifndef NBT_TAG_END_H
# define NBT_TAG_END_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagEnd : public NBTTag
	{

	public:
		NBTTagEnd();
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		void writeNameToFile(NBTFile *file) {(void)file;};
		void printDebug();

	};

}

#endif
