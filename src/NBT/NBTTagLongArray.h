#ifndef NBT_TAG_LONG_ARRAY_H
# define NBT_TAG_LONG_ARRAY_H

# include "NBTTagInt.h"
# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagLongArray : public NBTTag
	{

	private:
		std::vector<int64_t> values;

	public:
		NBTTagLongArray(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug(size_t tab = 0);
		inline std::vector<int64_t> &getValues() {return this->values;};

	};

}

#endif
