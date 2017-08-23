#ifndef CHUNK_H
# define CHUNK_H

# include "Block.h"
# include <librender/Shader/DataBuffer.h>

using librender::DataBuffer;

# define CHUNK_WIDTH 16
# define CHUNK_HEIGHT 256

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
		Block **blocks;
		Chunk *chunkXLess;
		Chunk *chunkXMore;
		Chunk *chunkZLess;
		Chunk *chunkZMore;
		World &world;
		uint32_t verticesNb;
		int32_t x;
		int32_t z;
		bool mustGenerateBuffers;

	public:
		Chunk(World &world, int32_t x, int32_t z);
		~Chunk();
		void generateGLBuffer();
		void draw();
		inline Block **getBlocks() {return (this->blocks);};
		inline void setChunkXLess(Chunk *chunk);
		inline Chunk *getChunkXLess() {return (this->chunkXLess);};
		inline void setChunkXMore(Chunk *chunk);
		inline Chunk *getChunkXMore() {return (this->chunkXMore);};
		inline void setChunkZLess(Chunk *chunk);
		inline Chunk *getChunkZLess() {return (this->chunkZLess);};
		inline void setChunkZMore(Chunk *chunk);
		inline Chunk *getChunkZMore() {return (this->chunkZMore);};
		inline Block *getBlockAt(int32_t x, int32_t y, int32_t z) {return (this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z]);};
		inline World &getWorld() {return (this->world);};
		inline int32_t getX() {return (this->x);};
		inline int32_t getZ() {return (this->z);};

	};

}

#endif
