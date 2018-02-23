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
		virtual void readData(NBTStream *stream) {(void)stream;};
		virtual void writeData(NBTStream *stream) {(void)stream;};
		void writeId(NBTStream *stream);
		virtual void writeName(NBTStream *stream);
		size_t getHeaderSize();
		virtual size_t getDataSize() {return (0);};
		virtual void printDebug();
		inline enum NBTTagType getType() {return (this->typeId);};
		inline std::string &getName() {return (this->name);};
		static NBTTag *readTag(NBTStream *stream);
		static std::string readTagName(NBTStream *stream);
		static NBTTag *readTagOfType(enum NBTTagType type, std::string name);

	};

}

#endif
