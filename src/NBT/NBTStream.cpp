#include "NBTStream.h"

namespace voxel
{

	bool NBTStream::readInt8(int8_t *data)
	{
		return (read(data, 1));
	}

	bool NBTStream::readInt16(int16_t *data)
	{
		if (!read(data, 2))
			return (false);
#if __BYTE_ORDER == __LITTLE_ENDIAN
		*(reinterpret_cast<uint16_t*>(data)) = b_htons(*reinterpret_cast<uint16_t*>(data));
#endif
		return (true);
	}

	bool NBTStream::readInt32(int32_t *data)
	{
		if (!read(data, 4))
			return (false);
#if __BYTE_ORDER == __LITTLE_ENDIAN
		*(reinterpret_cast<uint32_t*>(data)) = b_htonl(*reinterpret_cast<uint32_t*>(data));
#endif
		return (true);
	}

	bool NBTStream::readInt64(int64_t *data)
	{
		if (!read(data, 8))
			return (false);
#if __BYTE_ORDER == __LITTLE_ENDIAN
		*(reinterpret_cast<uint64_t*>(data)) = b_htonll(*reinterpret_cast<uint64_t*>(data));
#endif
		return (true);
	}

	bool NBTStream::readFloat(float *data)
	{
		if (!read(data, 4))
			return (false);
#if __BYTE_ORDER == __LITTLE_ENDIAN
		*(reinterpret_cast<uint32_t*>(data)) = b_htonl(*reinterpret_cast<uint32_t*>(data));
#endif
		return (true);
	}

	bool NBTStream::readDouble(double *data)
	{
		if (!read(data, 8))
			return (false);
#if __BYTE_ORDER == __LITTLE_ENDIAN
		*(reinterpret_cast<uint64_t*>(data)) = b_htonll(*reinterpret_cast<uint64_t*>(data));
#endif
		return (true);
	}

	bool NBTStream::writeInt8(int8_t value)
	{
		return (write(&value, 1));
	}

	bool NBTStream::writeInt16(int16_t value)
	{
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uint16_t val = b_htons(*reinterpret_cast<uint16_t*>(&value));
		return (write(&val, 2));
#else
		return (write(&value, 2));
#endif
	}

	bool NBTStream::writeInt32(int32_t value)
	{
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uint32_t val = b_htonl(*reinterpret_cast<uint32_t*>(&value));
		return (write(&val, 4));
#else
		return (write(&value, 4));
#endif
	}

	bool NBTStream::writeInt64(int64_t value)
	{
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uint64_t val = b_htonll(*reinterpret_cast<uint64_t*>(&value));
		return (write(&val, 8));
#else
		return (write(&value, 8));
#endif
	}

	bool NBTStream::writeFloat(float value)
	{
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uint32_t *tmp = reinterpret_cast<uint32_t*>(&value);
		uint32_t val = b_htonl(*tmp);
		return (write(&val, 4));
#else
		return (write(&value, 4));
#endif
	}

	bool NBTStream::writeDouble(double value)
	{
#if __BYTE_ORDER == __LITTLE_ENDIAN
		uint64_t *tmp = reinterpret_cast<uint64_t*>(&value);
		uint64_t val = b_htonll(*tmp);
		return (write(&val, 8));
#else
		return (write(&value, 8));
#endif
	}

	uint16_t NBTStream::b_htons(uint16_t val)
	{
		return (((val & 0xFF) << 8) | ((val & 0xFF00) >> 8));
	}

	uint32_t NBTStream::b_htonl(uint32_t val)
	{
		return (((val & 0xFF) << 24) | ((val & 0xFF00) << 8) | ((val & 0xFF0000) >> 8) | ((val & 0xFF000000) >> 24));
	}

	uint64_t NBTStream::b_htonll(uint64_t val)
	{
		return (((val & 0xFF) << 56) | ((val & 0xFF00) << 40) | ((val & 0xFF0000) << 24) | ((val & 0xFF000000) << 8)
				| ((val & 0xFF00000000) >> 8) | ((val & 0xFF0000000000) >> 24) | ((val & 0xFF000000000000) >> 40) | ((val & 0xFF00000000000000) >> 56));
	}

}
