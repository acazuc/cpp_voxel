#ifndef REGION_H
# define REGION_H

# include "./Chunk.h"

# define REGION_WIDTH 32

namespace voxel
{

	class World;

	class Region
	{

	private:
		Chunk *chunks[REGION_WIDTH * REGION_WIDTH];
		World &world;
		int32_t x;
		int32_t z;

	public:
		Region(World &world, int32_t x, int32_t z);
		~Region();
		void save();
		void moveGLBuffersToWorld();
		void tick();
		void drawEntities();
		void draw(uint8_t layer);
		void generateChunk(int32_t x, int32_t z);
		void setChunk(int32_t x, int32_t z, Chunk *chunk);
		Chunk *getChunk(int32_t x, int32_t z);
		inline Chunk **getChunks() {return (this->chunks);};
		inline int32_t getX() {return (this->x);};
		inline int32_t getZ() {return (this->z);};

	};

}

#endif
