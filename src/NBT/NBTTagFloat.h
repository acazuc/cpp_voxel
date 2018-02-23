#ifndef NBT_TAG_FLOAT_H
# define NBT_TAG_FLOAT_H

# include "NBTTag.h"

namespace voxel
{

	class NBTTagFloat : public NBTTag
	{

	private:
		float value;

	public:
		NBTTagFloat(std::string name);
		void readData(NBTStream *stream);
		void writeData(NBTStream *stream);
		size_t getDataSize();
		void printDebug();
		inline float getValue() {return (this->value);};

	};

}

#endif
