#ifndef CHUNK_H
# define CHUNK_H

# include "ChunkBlock.h"
# include "AABB.h"
# include <librender/Shader/VertexBuffer.h>

using librender::VertexBuffer;

# define CHUNK_WIDTH 16
# define CHUNK_HEIGHT 255

namespace voxel
{

	class World;

	class Chunk
	{

	private:
		VertexBuffer *texCoordsBuffer;
		VertexBuffer *vertexesBuffer;
		VertexBuffer *indicesBuffer;
		VertexBuffer *colorsBuffer;
		ChunkBlock *blocks;
		glm::mat4 modelMat;
		Chunk *chunkXLess;
		Chunk *chunkXMore;
		Chunk *chunkZLess;
		Chunk *chunkZMore;
		World &world;
		AABB aabb;
		uint32_t verticesNb;
		uint8_t *topBlocks;
		uint8_t *lightMap;
		int32_t x;
		int32_t z;
		bool mustGenerateLightMap;
		bool mustGenerateBuffers;
		bool deleted;
		void generateGLBuffer();
		void generateLightMap();
		void setBlockLightRec(glm::vec3 pos, uint8_t light);

	public:
		Chunk(World &world, int32_t x, int32_t z);
		~Chunk();
		void draw();
		void addBlock(glm::vec3 pos, uint8_t type);
		void destroyBlock(glm::vec3 pos);
		inline ChunkBlock **getBlocks() {return (&this->blocks);};
		inline void setChunkXLess(Chunk *chunk);
		inline Chunk *getChunkXLess() {return (this->chunkXLess);};
		inline void setChunkXMore(Chunk *chunk);
		inline Chunk *getChunkXMore() {return (this->chunkXMore);};
		inline void setChunkZLess(Chunk *chunk);
		inline Chunk *getChunkZLess() {return (this->chunkZLess);};
		inline void setChunkZMore(Chunk *chunk);
		inline Chunk *getChunkZMore() {return (this->chunkZMore);};
		inline ChunkBlock *getBlockAt(glm::vec3 pos) {return (&this->blocks[getXYZId(pos)]);};
		inline uint8_t getLightAt(glm::vec3 pos) {return (this->lightMap[getXYZId(pos)]);};
		inline uint16_t getTopBlockAt(int32_t x, int32_t z) {return (this->topBlocks[getXZId(x, z)]);};
		inline World &getWorld() {return (this->world);};
		inline int32_t getX() {return (this->x);};
		inline int32_t getZ() {return (this->z);};
		inline void regenerateLightMap() {this->mustGenerateLightMap = true;};
		inline void regenerateBuffers() {this->mustGenerateBuffers = true;};
		inline void setDeleted(bool deleted) {this->deleted = deleted;};
		inline bool isDeleted() {return (this->deleted);};
		inline void setTexCoordsBuffer(VertexBuffer *texCoordsBuffer) {this->texCoordsBuffer = texCoordsBuffer;};
		inline VertexBuffer *getTexCoordsBuffer() {return (this->texCoordsBuffer);};
		inline void setVertexesBuffer(VertexBuffer *vertexesBuffer) {this->vertexesBuffer = vertexesBuffer;};
		inline VertexBuffer *getVertexesBuffer() {return (this->vertexesBuffer);};
		inline void setIndicesBuffer(VertexBuffer *indicesBuffer) {this->indicesBuffer = indicesBuffer;};
		inline VertexBuffer *getIndicesBuffer() {return (this->indicesBuffer);};
		inline void setColorsBuffer(VertexBuffer *colorsBuffer) {this->colorsBuffer = colorsBuffer;};
		inline VertexBuffer *getColorsBuffer() {return (this->colorsBuffer);};
		inline int32_t getXYZId(glm::vec3 pos) {return ((pos.x * CHUNK_HEIGHT + pos.y) * CHUNK_WIDTH + pos.z);};
		inline int32_t getXZId(int32_t x, int32_t z) {return (x * CHUNK_WIDTH + z);};

	};

}

#endif
