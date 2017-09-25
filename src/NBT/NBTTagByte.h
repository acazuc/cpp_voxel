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
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		inline int8_t getValue() {return (this->value);};

	};

}

#endif
