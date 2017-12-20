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
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		void printDebug();
		inline int32_t getValue() {return (this->value);};

	};

}

#endif
