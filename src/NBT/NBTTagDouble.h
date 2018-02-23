#ifndef NBT_TAG_DOUBLE_H
# define NBT_TAG_DOUBLE_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagDouble : public NBTTag
	{

	private:
		double value;

	public:
		NBTTagDouble(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug();
		inline double getValue() {return (this->value);};

	};

}

#endif
