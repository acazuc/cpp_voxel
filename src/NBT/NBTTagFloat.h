#ifndef NBT_TAG_FLOAT_H
# define NBT_TAG_FLOAT_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagFloat : public NBTTag
	{

	private:
		float value;

	public:
		NBTTagFloat(std::string name);
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		void printDebug();
		inline float getValue() {return (this->value);};

	};

}

#endif
