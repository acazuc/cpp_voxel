#ifndef NBT_TAG_H
# define NBT_TAG_H

# include "NBTTagType.h"
# include "NBTStream.h"
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
		virtual ~NBTTag();
		virtual void readData(NBTStream *stream) = 0;
		virtual void writeData(NBTStream *stream) = 0;
		void writeId(NBTStream *stream);
		virtual void writeName(NBTStream *stream);
		void writeHeader(NBTStream *stream);
		size_t getHeaderSize();
		virtual size_t getDataSize() = 0;
		virtual void printDebug(size_t tab = 0) = 0;
		inline enum NBTTagType getType() {return this->typeId;};
		inline std::string &getName() {return this->name;};
		static NBTTag *readTag(NBTStream *stream);
		static std::string readTagName(NBTStream *stream);
		static NBTTag *getTagOfType(enum NBTTagType type, std::string name);

	};

}

#endif
