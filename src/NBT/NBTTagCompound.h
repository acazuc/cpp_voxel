#ifndef NBT_TAG_COMPOUND_H
# define NBT_TAG_COMPOUND_H

# include "NBTTag.h"
# include <vector>

namespace voxel
{

	class NBTTagCompound : public NBTTag
	{

	private:
		std::vector<NBTTag*> tags;

	public:
		NBTTagCompound(std::string name);
		~NBTTagCompound();
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug(size_t tab = 0);
		inline void addTag(NBTTag *tag) {this->tags.push_back(tag);};
		inline std::vector<NBTTag*> &getTags() {return this->tags;};

	};

}

#endif
