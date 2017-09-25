#ifndef NBT_TAG_LONG_ARRAY_H
# define NBT_TAG_LONG_ARRAY_H

# include "NBTTagInt.h"
# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagLongArray : public NBTTag
	{

	private:
		std::vector<int64_t> values;
		NBTTagInt *size;

	public:
		NBTTagLongArray(std::string name);
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		inline std::vector<int64_t> &getValues() {return (this->values);};
		inline NBTTagInt *getSize() {return (this->size);};

	};

}

#endif
