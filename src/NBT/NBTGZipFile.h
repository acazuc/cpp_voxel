#ifndef NBT_GZIP_FILE_H
# define NBT_GZIP_FILE_H

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

	class NBTGZipFile : public NBTStream
	{

	private:
		std::vector<NBTTag*> tags;
		GZipOutputStream ostream;
		GZipInputStream istream;
		std::string name;

	public:
		NBTGZipFile(std::string name);
		void load();
		void save();
		void printDebug();
		bool read(void *data, size_t len);
		bool write(void *data, size_t len);

	};

}

#endif
