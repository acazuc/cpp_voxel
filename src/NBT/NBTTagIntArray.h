#ifndef NBT_TAG_INT_ARRAY_H
# define NBT_TAG_INT_ARRAY_H

# include "NBTTagInt.h"
# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagIntArray : public NBTTag
	{

	private:
		std::vector<int32_t> values;

	public:
		NBTTagIntArray(std::string name);
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		inline std::vector<int32_t> &getValues() {return (this->values);};

	};

}

#endif
