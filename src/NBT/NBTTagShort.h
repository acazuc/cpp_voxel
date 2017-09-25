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
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		inline int16_t getValue() {return (this->value);};

	};

}

#endif
