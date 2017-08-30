#ifndef CHUNK_H
# define CHUNK_H

# include "Block.h"
# include <librender/Shader/DataBuffer.h>

using librender::DataBuffer;

# define CHUNK_WIDTH 16
# define CHUNK_HEIGHT 255

namespace voxel
{

	class World;

	class Chunk
	{

	private:
		DataBuffer *texCoordsBuffer;
		DataBuffer *vertexesBuffer;
		DataBuffer *indicesBuffer;
		DataBuffer *colorsBuffer;
		glm::mat4 modelMat;
		Block *blocks;
		Chunk *chunkXLess;
		Chunk *chunkXMore;
		Chunk *chunkZLess;
		Chunk *chunkZMore;
		World &world;
		uint32_t verticesNb;
		uint8_t *topBlocks;
		uint8_t *lightMap;
		int32_t x;
		int32_t z;
		bool mustGenerateBuffers;
		bool deleted;
		void generateGLBuffer();
		void generateLightMap();
		void setBlockLightRec(int32_t x, int32_t y, int32_t z, uint8_t light);

	public:
		Chunk(World &world, int32_t x, int32_t z);
		~Chunk();
		void draw();
		void addBlock(int32_t x, int32_t y, int32_t z, uint8_t type);
		void destroyBlock(int32_t x, int32_t y, int32_t z);
		inline Block **getBlocks() {return (&this->blocks);};
		inline void setChunkXLess(Chunk *chunk);
		inline Chunk *getChunkXLess() {return (this->chunkXLess);};
		inline void setChunkXMore(Chunk *chunk);
		inline Chunk *getChunkXMore() {return (this->chunkXMore);};
		inline void setChunkZLess(Chunk *chunk);
		inline Chunk *getChunkZLess() {return (this->chunkZLess);};
		inline void setChunkZMore(Chunk *chunk);
		inline Chunk *getChunkZMore() {return (this->chunkZMore);};
		inline Block *getBlockAt(int32_t x, int32_t y, int32_t z) {return (&this->blocks[getXYZId(x, y, z)]);};
		inline uint8_t getLightAt(int32_t x, int32_t y, int32_t z) {return (this->lightMap[getXYZId(x, y, z)]);};
		inline uint16_t getTopBlockAt(int32_t x, int32_t z) {return (this->topBlocks[getXZId(x, z)]);};
		inline World &getWorld() {return (this->world);};
		inline int32_t getX() {return (this->x);};
		inline int32_t getZ() {return (this->z);};
		inline void regenerateBuffers() {this->mustGenerateBuffers = true;};
		inline void setDeleted(bool deleted) {this->deleted = deleted;};
		inline bool isDeleted() {return (this->deleted);};
		inline int32_t getXYZId(int32_t x, int32_t y, int32_t z) {return ((x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z);};
		inline int32_t getXZId(int32_t x, int32_t z) {return (x * CHUNK_WIDTH + z);};

	};

}

#endif
