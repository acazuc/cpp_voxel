#ifndef NBT_TAG_LONG_H
# define NBT_TAG_LONG_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagLong : public NBTTag
	{

	private:
		int64_t value;

	public:
		NBTTagLong(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug();
		inline void setValue(int64_t value) {this->value = value;};
		inline int64_t getValue() {return (this->value);};

	};

}

#endif
