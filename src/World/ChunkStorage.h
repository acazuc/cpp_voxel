#ifndef CHUNK_STORAGE_H
# define CHUNK_STORAGE_H

# include "ChunkBlock.h"
# include "NBT/NBT.h"

namespace voxel
{

	class Chunk;

	struct ChunkStorageNBT
	{
		NBTTagCompound *NBT;
		NBTTagByte *Y;
		NBTTagByteArray *Blocks;
		NBTTagByteArray *Add;
		NBTTagByteArray *Data;
		NBTTagByteArray *BlockLight;
		NBTTagByteArray *SkyLight;
	};

	class ChunkStorage
	{

	private:
		ChunkStorageNBT NBT;
		uint8_t id;

	public:
		ChunkStorage(uint8_t id);
		~ChunkStorage();
		void initNBT(NBTTagCompound *compound);
		void fillBuffers(Chunk *chunk, ChunkTessellator &tessellator, uint8_t layer);
		void resetLights();
		void setBlock(int32_t x, int32_t y, int32_t z, uint8_t type);
		ChunkBlock *getBlock(int32_t x, int32_t y, int32_t z);
		uint8_t getLight(int32_t x, int32_t y, int32_t z);
		void setSkyLight(int32_t x, int32_t y, int32_t z, uint8_t light);
		uint8_t getSkyLight(int32_t x, int32_t y, int32_t z);
		void setBlockLight(int32_t x, int32_t y, int32_t z, uint8_t light);
		uint8_t getBlockLight(int32_t x, int32_t y, int32_t z);
		uint32_t getXYZId(int8_t x, int8_t y, int8_t z);
		inline uint8_t getId() {return (this->id);};

	};

}

#endif
