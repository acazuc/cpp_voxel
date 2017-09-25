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
# include "NBTTag.h"
# include <libgzip/GZipInputStream.h>
# include <string>
# include <vector>

using libgzip::GZipInputStream;

namespace voxel
{

	class NBTFile
	{

	private:
		std::vector<NBTTag*> tags;
		GZipInputStream istream;
		std::string name;
		uint16_t htons(uint16_t val);
		uint32_t htonl(uint32_t val);
		uint64_t htonll(uint64_t val);

	public:
		NBTFile(std::string name);
		void load();
		void save();
		bool readInt8(int8_t *data);
		bool readInt16(int16_t *data);
		bool readInt32(int32_t *data);
		bool readInt64(int64_t *data);
		bool readFloat(float *data);
		bool readDouble(double *data);
		bool readData(void *data, size_t len);
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
