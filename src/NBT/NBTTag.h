#ifndef NBT_TAG_H
# define NBT_TAG_H

# include "NBTTagType.h"
# include <string>

namespace voxel
{

	class NBTFile;

	class NBTTag
	{

	protected:
		enum NBTTagType typeId;
		std::string name;

	public:
		NBTTag(enum NBTTagType type, std::string name);
		virtual void readDataFromFile(NBTFile *file) {(void)file;};
		virtual void writeDataToFile(NBTFile *file) {(void)file;};
		inline enum NBTTagType getType() {return (this->typeId);};
		inline std::string &getName() {return (this->name);};

	};

}

#endif
