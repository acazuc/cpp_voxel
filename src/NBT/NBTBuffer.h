#ifndef NBT_BUFFER_H
# define NBT_BUFFER_H

# include "NBTStream.h"

namespace voxel
{

	class NBTBuffer : public NBTStream
	{

	public:
		void *data;
		size_t len;
		size_t pos;
		bool read(void *data, size_t len);
		bool write(const void *data, size_t len);

	};

}

#endif
