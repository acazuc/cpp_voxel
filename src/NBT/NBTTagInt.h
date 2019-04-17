#ifndef NBT_TAG_INT_H
# define NBT_TAG_INT_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagInt : public NBTTag
	{

	private:
		int32_t value;

	public:
		NBTTagInt(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug(size_t tab = 0);
		inline void setValue(int32_t value) {this->value = value;};
		inline int32_t getValue() {return this->value;};

	};

}

#endif
