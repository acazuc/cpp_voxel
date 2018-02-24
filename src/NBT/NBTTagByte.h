#ifndef NBT_TAG_BYTE_H
# define NBT_TAG_BYTE_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagByte : public NBTTag
	{

	private:
		int8_t value;

	public:
		NBTTagByte(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug();
		inline void setValue(int8_t value) {this->value = value;};
		inline int8_t getValue() {return (this->value);};

	};

}

#endif
