#ifndef NBT_TAG_BYTE_ARRAY_H
# define NBT_TAG_BYTE_ARRAY_H

# include "NBTTagInt.h"
# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagByteArray : public NBTTag
	{

	private:
		std::vector<int8_t> values;

	public:
		NBTTagByteArray(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug(size_t tab = 0);
		inline std::vector<int8_t> &getValues() {return this->values;};

	};

}

#endif
