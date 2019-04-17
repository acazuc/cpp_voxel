#ifndef NBT_TAG_STRING_H
# define NBT_TAG_STRING_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagString : public NBTTag
	{

	private:
		std::string value;

	public:
		NBTTagString(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug(size_t tab = 0);
		inline void setValue(std::string value) {this->value = value;};
		inline std::string &getValue() {return this->value;};

	};

}

#endif
