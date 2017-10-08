#ifndef NBT_TAG_LIST_H
# define NBT_TAG_LIST_H

# include "NBTTagByte.h"
# include "NBTTagInt.h"
# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagList : public NBTTag
	{

	private:
		std::vector<NBTTag*> values;
		enum NBTTagType type;

	public:
		NBTTagList(std::string name);
		void readDataFromFile(NBTFile *file);
		void writeDataToFile(NBTFile *file);
		inline std::vector<NBTTag*> &getValues() {return (this->values);};
		inline enum NBTTagType getType() {return (this->type);};

	};

}

#endif