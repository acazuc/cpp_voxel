#ifndef CHUNK_H
# define CHUNK_H

# include "Block.h"
# include <librender/Shader/DataBuffer.h>

using librender::DataBuffer;

# define CHUNK_WIDTH 16
# define CHUNK_HEIGHT 128

namespace voxel
{

	class World;

	class Chunk
	{

	private:
		DataBuffer texCoordsBuffer;
		DataBuffer vertexesBuffer;
		DataBuffer indicesBuffer;
		DataBuffer colorsBuffer;
		Block ****blocks;
		Chunk *chunkXLess;
		Chunk *chunkXMore;
		Chunk *chunkZLess;
		Chunk *chunkZMore;
		World *world;
		uint32_t verticesNb;
		int32_t x;
		int32_t y;

	public:
		Chunk(World *world, int32_t x, int32_t y);
		void generateGLBuffer();
		void draw();
		inline Block ****getBlocks() {return (this->blocks);};
		inline Chunk *getChunkXLess() {return (this->chunkXLess);};
		inline Chunk *getChunkXMore() {return (this->chunkXMore);};
		inline Chunk *getChunkZLess() {return (this->chunkZLess);};
		inline Chunk *getChunkZMore() {return (this->chunkZMore);};
		inline Block *getBlockAt(int32_t x, int32_t y, int32_t z) {return (this->blocks[x][y][z]);};
		inline World *getWorld() {return (this->world);};

	};

}

#endif
