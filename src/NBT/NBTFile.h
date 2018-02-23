#ifndef NBT_FILE_H
# define NBT_FILE_H

# include "NBTTagByteArray.h"
# include "NBTTagLongArray.h"
# include "NBTTagIntArray.h"
# include "NBTTagCompound.h"
# include "NBTTagDouble.h"
# include "NBTTagString.h"
# include "NBTTagShort.h"
# include "NBTTagFloat.h"
# include "NBTTagByte.h"
# include "NBTTagLong.h"
# include "NBTTagList.h"
# include "NBTTagEnd.h"
# include "NBTTagInt.h"
# include "NBTStream.h"
# include "NBTTag.h"
# include <libgzip/GZipOutputStream.h>
# include <libgzip/GZipInputStream.h>
# include <string>
# include <vector>

using libgzip::GZipOutputStream;
using libgzip::GZipInputStream;

namespace voxel
{

	class NBTFile : public NBTStream
	{

	private:
		std::vector<NBTTag*> tags;
		GZipOutputStream ostream;
		GZipInputStream istream;
		std::string name;

	public:
		NBTFile(std::string name);
		void load();
		void save();
		void printDebug();
		bool read(void *data, size_t len);
		bool write(void *data, size_t len);
		std::string readTagName();
		NBTTag *readNextTag();
		NBTTag *readTagOfType(enum NBTTagType type, std::string name);
		NBTTagEnd *readTagEnd();
		NBTTagByte *readTagByte(std::string name);
		NBTTagShort *readTagShort(std::string name);
		NBTTagInt *readTagInt(std::string name);
		NBTTagLong *readTagLong(std::string name);
		NBTTagFloat *readTagFloat(std::string name);
		NBTTagDouble *readTagDouble(std::string name);
		NBTTagByteArray *readTagByteArray(std::string name);
		NBTTagString *readTagString(std::string name);
		NBTTagList *readTagList(std::string name);
		NBTTagCompound *readTagCompound(std::string name);
		NBTTagIntArray *readTagIntArray(std::string name);
		NBTTagLongArray *readTagLongArray(std::string name);

	};

}

#endif
