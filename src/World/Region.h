#ifndef REGION_H
# define REGION_H

# include "./Chunk.h"

# define REGION_WIDTH 32
# define REGION_SECTOR_SIZE 4096

namespace voxel
{

	class World;

	class Region
	{

	private:
		std::vector<bool> sectors;
		std::string filename;
		Chunk *chunks[REGION_WIDTH * REGION_WIDTH];
		World &world;
		uint32_t storageTimestamp[REGION_WIDTH * REGION_WIDTH];
		uint32_t storageHeader[REGION_WIDTH * REGION_WIDTH];
		int32_t x;
		int32_t z;
		FILE *file;

	public:
		Region(World &world, int32_t x, int32_t z);
		~Region();
		bool checkClear();
		void load();
		void save();
		void saveChunk(Chunk *chunk);
		void moveGLBuffersToWorld();
		void tick();
		void drawEntities();
		void draw(uint8_t layer);
		void generateChunk(int32_t x, int32_t z);
		Chunk *createChunk(int32_t x, int32_t z);
		void setChunk(int32_t x, int32_t z, Chunk *chunk);
		Chunk *getChunk(int32_t x, int32_t z);
		uint32_t getXZId(int32_t x, int32_t z);
		inline Chunk **getChunks() {return this->chunks;};
		inline int32_t getX() {return this->x;};
		inline int32_t getZ() {return this->z;};

	};

}

#endif
