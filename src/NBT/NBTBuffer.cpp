#include "NBTBuffer.h"

namespace voxel
{

	bool NBTBuffer::read(void *data, size_t len)
	{
		if (this->len - this->pos < len)
			return (false);
		std::memcpy(data, this->data + this->pos, len);
		this->pos += len;
		return (true);
	}

	bool NBTBuffer::write(const void *data, size_t len)
	{
		if (this->pos - this->pos < len)
			return (false);
		std::memcpy(this->data + this->pos, data, len);
		this->pos += len;
		return (true);
	}

}
