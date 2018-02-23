#ifndef NBT_TAG_END_H
# define NBT_TAG_END_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagEnd : public NBTTag
	{

	public:
		NBTTagEnd();
		void readData(NBTStream *stream) {(void)stream;};
		void writeData(NBTStream *stream) {(void)stream;};
		void writeName(NBTStream *stream) {(void)stream;};
		size_t getDataSize() {return (0);};
		void printDebug();

	};

}

#endif
