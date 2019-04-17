#include "NBTBuffer.h"

namespace voxel
{

	bool NBTBuffer::read(void *data, size_t len)
	{
		if (this->pos + len > this->len)
			return false;
		std::memcpy(data, reinterpret_cast<uint8_t*>(this->data) + this->pos, len);
		this->pos += len;
		return true;
	}

	bool NBTBuffer::write(const void *data, size_t len)
	{
		if (this->pos + len > this->len)
			return false;
		std::memcpy(reinterpret_cast<uint8_t*>(this->data) + this->pos, data, len);
		this->pos += len;
		return true;
	}

}
