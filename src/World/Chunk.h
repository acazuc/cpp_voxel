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

	struct ChunkLayer
	{
		ChunkTessellator tessellator;
		VertexBuffer *texCoordsBuffer;
		VertexBuffer *vertexesBuffer;
		VertexBuffer *indicesBuffer;
		VertexBuffer *colorsBuffer;
		uint32_t verticesNb;
		ChunkLayer() : texCoordsBuffer(NULL), vertexesBuffer(NULL), indicesBuffer(NULL), colorsBuffer(NULL), verticesNb(0) {};
	};

	class Chunk
	{

	private:
		ChunkLayer layers[3];
		ChunkBlock *blocks;
		glm::mat4 modelMat;
		Chunk *chunkXLess;
		Chunk *chunkXMore;
		Chunk *chunkZLess;
		Chunk *chunkZMore;
		World &world;
		AABB aabb;
		uint8_t *topBlocks;
		uint8_t *lightMap;
		int32_t x;
		int32_t z;
		bool mustGenerateLightMap;
		bool mustGenerateBuffers;
		bool mustUpdateBuffers;
		bool deleted;
		bool visible;
		void updateGLBuffers();
		void setBlockLightRec(glm::vec3 pos, uint8_t light);

	public:
		Chunk(World &world, int32_t x, int32_t z);
		~Chunk();
		void tick();
		void draw(uint8_t layer);
		void generateBuffers();
		void regenerateBuffers();
		void generateLightMap();
		void regenerateLightMap();
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
		inline void setDeleted(bool deleted) {this->deleted = deleted;};
		inline bool isDeleted() {return (this->deleted);};
		inline ChunkLayer &getLayer(uint8_t layer) {return (this->layers[layer]);};
		inline int32_t getXYZId(glm::vec3 pos) {return (((int32_t)pos.x * CHUNK_HEIGHT + (int32_t)pos.y) * CHUNK_WIDTH + (int32_t)pos.z);};
		inline int32_t getXZId(int32_t x, int32_t z) {return (x * CHUNK_WIDTH + z);};
		inline bool isMustGenerateLightMap() {return (this->mustGenerateLightMap);};
		inline bool isMustGenerateBuffers() {return (this->mustGenerateBuffers);};

	};

}

#endif
