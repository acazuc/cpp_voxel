#ifndef NBT_TAG_BYTE_ARRAY_H
# define NBT_TAG_BYTE_ARRAY_H

# include "NBTTagInt.h"
# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagByteArray : public NBTTag
	{

	private:
		std::vector<int8_t> values;
		NBTTagInt *size;

	public:
		NBTTagByteArray(std::string name);
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		inline std::vector<int8_t> &getValues() {return (this->values);};
		inline NBTTagInt *getSize() {return (this->size);};

	};

}

#endif
