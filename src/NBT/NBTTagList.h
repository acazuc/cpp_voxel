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
		~NBTTagList();
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug();
		inline void addValue(NBTTag* value) {this->values.push_back(value);};
		inline std::vector<NBTTag*> &getValues() {return (this->values);};
		inline void setType(enum NBTTagType type) {this->type = type;};
		inline enum NBTTagType getType() {return (this->type);};

	};

}

#endif
