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
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		void printDebug();
		inline std::string &getValue() {return (this->value);};

	};

}

#endif
