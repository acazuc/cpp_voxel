#ifndef NBT_STREAM_H
# define NBT_STREAM_H

# include <cstdint>
# include <cstring>

namespace voxel
{

	class NBTStream
	{

	public:
		virtual ~NBTStream() {};
		bool readInt8(int8_t *data);
		bool readInt16(int16_t *data);
		bool readInt32(int32_t *data);
		bool readInt64(int64_t *data);
		bool readFloat(float *data);
		bool readDouble(double *data);
		virtual bool read(void *data, size_t len) {(void)data;(void)len;return (false);};
		bool writeInt8(int8_t value);
		bool writeInt16(int16_t value);
		bool writeInt32(int32_t value);
		bool writeInt64(int64_t value);
		bool writeFloat(float value);
		bool writeDouble(double value);
		virtual bool write(const void *data, size_t len) {(void)data;(void)len;return (false);};
		uint16_t b_htons(uint16_t val);
		uint32_t b_htonl(uint32_t val);
		uint64_t b_htonll(uint64_t val);
	};

}

#endif
