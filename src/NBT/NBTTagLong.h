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
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		void printDebug();
		inline int64_t getValue() {return (this->value);};

	};

}

#endif
