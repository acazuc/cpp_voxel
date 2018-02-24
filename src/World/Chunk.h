#ifndef CHUNK_H
# define CHUNK_H

# include "Particles/ParticlesManager.h"
# include "Entities/EntitiesManager.h"
# include "ChunkStorage.h"
# include "NBT/NBT.h"
# include "AABB.h"
# include <librender/Shader/VertexBuffer.h>
# include <librender/Shader/VertexArray.h>

using librender::VertexBuffer;
using librender::VertexArray;

# define CHUNK_WIDTH 16
# define CHUNK_HEIGHT 255

namespace voxel
{

	class World;

	struct ChunkNBT
	{
		NBTTagCompound *NBT;
		NBTTagInt *DataVersion;
		NBTTagCompound *Level;
		NBTTagInt *xPos;
		NBTTagInt *zPos;
		NBTTagLong *LastUpdate;
		NBTTagByte *LightPopulated;
		NBTTagByte *TerrainPopulated;
		NBTTagByte *V;
		NBTTagLong *InhabitedTime;
		NBTTagByteArray *Biomes;
		NBTTagByteArray *HeightMap;
		NBTTagList *Sections;
		NBTTagList *Entities;
		NBTTagList *TileEntities;
		NBTTagList *TileTicks;
	};

	struct ChunkLayer
	{
		ChunkTessellator tessellator;
		VertexBuffer *texCoordsBuffer;
		VertexBuffer *vertexesBuffer;
		VertexBuffer *indicesBuffer;
		VertexBuffer *colorsBuffer;
		VertexArray *vertexArray;
		uint32_t verticesNb;
		ChunkLayer() : texCoordsBuffer(NULL), vertexesBuffer(NULL), indicesBuffer(NULL), colorsBuffer(NULL), vertexArray(NULL), verticesNb(0) {};
	};

	class Chunk
	{

	private:
		ParticlesManager particlesManager;
		EntitiesManager entitiesManager;
		ChunkStorage *storages[16];
		ChunkLayer layers[3];
		ChunkNBT NBT;
		Chunk *chunkXLess;
		Chunk *chunkXMore;
		Chunk *chunkZLess;
		Chunk *chunkZMore;
		World &world;
		AABB aabb;
		int32_t x;
		int32_t z;
		bool mustGenerateLightMap;
		bool mustGenerateBuffers;
		bool mustUpdateBuffers;
		bool recursiveLightMap;
		bool deleted;
		bool changed;
		bool visible;
		void updateGLBuffer(uint8_t layer);
		void updateGLBuffers();
		void setBlockLightRec(int32_t x, int32_t y, int32_t z, uint8_t light);

	public:
		Chunk(World &world, int32_t x, int32_t z);
		~Chunk();
		void moveGLBuffersToWorld();
		void generate();
		void tick();
		void drawEntities();
		void draw(uint8_t layer);
		void generateBuffers();
		void regenerateBuffers();
		void generateLightMap();
		void regenerateLightMapRec();
		void regenerateLightMap();
		void setBlock(int32_t x, int32_t y, int32_t z, uint8_t type);
		void setBlockIfReplaceable(int32_t x, int32_t y, int32_t z, uint8_t type);
		ChunkBlock *getBlock(int32_t x, int32_t y, int32_t z);
		uint8_t getLight(int32_t x, int32_t y, int32_t z);
		void setSkyLight(int32_t x, int32_t y, int32_t z, uint8_t light);
		uint8_t getSkyLightVal(int32_t x, int32_t y, int32_t z);
		uint8_t getSkyLight(int32_t x, int32_t y, int32_t z);
		void setBlockLight(int32_t x, int32_t y, int32_t z, uint8_t light);
		uint8_t getBlockLight(int32_t x, int32_t y, int32_t z);
		void setTopBlock(int32_t x, int32_t y, uint8_t top);
		uint8_t getTopBlock(int32_t x, int32_t z);
		void setBiome(int32_t x, int32_t y, uint8_t biome);
		uint8_t getBiome(int32_t x, int32_t z);
		void destroyBlock(int32_t x, int32_t y, int32_t z);
		inline void setChunkXLess(Chunk *chunk);
		inline Chunk *getChunkXLess() {return (this->chunkXLess);};
		inline void setChunkXMore(Chunk *chunk);
		inline Chunk *getChunkXMore() {return (this->chunkXMore);};
		inline void setChunkZLess(Chunk *chunk);
		inline Chunk *getChunkZLess() {return (this->chunkZLess);};
		inline void setChunkZMore(Chunk *chunk);
		inline Chunk *getChunkZMore() {return (this->chunkZMore);};
		inline World &getWorld() {return (this->world);};
		inline int32_t getX() {return (this->x);};
		inline int32_t getZ() {return (this->z);};
		void setGenerated(bool generated);
		bool isGenerated();
		inline void setDeleted(bool deleted) {this->deleted = deleted;};
		inline bool isDeleted() {return (this->deleted);};
		inline void setChanged(bool changed) {this->changed = changed;};
		inline bool isChanged() {return (this->changed);};
		void initNBT(NBTTagCompound *NBT);
		inline NBTTagCompound *getNBT() {return (this->NBT.NBT);};
		ChunkStorage *getStorage(uint8_t id);
		ChunkStorage *createStorage(uint8_t id);
		inline ChunkStorage **getStorages() {return (this->storages);};
		inline ChunkLayer &getLayer(uint8_t layer) {return (this->layers[layer]);};
		inline int32_t getXYZId(int32_t x, int32_t y, int32_t z) {return ((x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z);};
		inline int32_t getXZId(int32_t x, int32_t z) {return (x * CHUNK_WIDTH + z);};
		inline bool isMustGenerateLightMap() {return (this->mustGenerateLightMap);};
		inline bool isMustGenerateBuffers() {return (this->mustGenerateBuffers);};
		inline ParticlesManager &getParticlesManager() {return (this->particlesManager);};
		inline EntitiesManager &getEntitiesManager() {return (this->entitiesManager);};

	};

}

#endif
