#ifndef NBT_TAG_INT_ARRAY_H
# define NBT_TAG_INT_ARRAY_H

# include "NBTTagInt.h"
# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagIntArray : public NBTTag
	{

	private:
		std::vector<int32_t> values;

	public:
		NBTTagIntArray(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug(size_t tab = 0);
		inline std::vector<int32_t> &getValues() {return this->values;};

	};

}

#endif
