#ifndef NBT_TAG_SHORT_H
# define NBT_TAG_SHORT_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagShort : public NBTTag
	{

	private:
		int16_t value;

	public:
		NBTTagShort(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug(size_t tab = 0);
		inline void setValue(int16_t value) {this->value = value;};
		inline int16_t getValue() {return this->value;};

	};

}

#endif
