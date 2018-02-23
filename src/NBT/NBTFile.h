#ifndef NBT_FILE_H
# define NBT_FILE_H

# include "NBTStream.h"
# include "NBTTag.h"
# include <fstream>
# include <string>
# include <vector>

namespace voxel
{

	class NBTFile : NBTStream
	{

	private:
		std::vector<NBTTag*> tags;
		std::ofstream ostream;
		std::ifstream istream;
		std::string name;

	public:
		NBTFile(std::string name);
		void load();
		void save();
		void printDebug();
		bool read(void *data, size_t len);
		bool write(void *data, size_t len);
	};

}

#endif
