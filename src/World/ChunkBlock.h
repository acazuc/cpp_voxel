#ifndef CHUNK_BLOCK_H
# define CHUNK_BLOCK_H

# include "ChunkTessellator.h"
# include "Blocks/Blocks.h"
# include "AABB.h"
# include <librender/GL.h>
# include <cstdint>
# include <vector>

# define BLOCK_FACE_FRONT	0x01
# define BLOCK_FACE_BACK	0x02
# define BLOCK_FACE_LEFT	0x04
# define BLOCK_FACE_RIGHT	0x08
# define BLOCK_FACE_UP		0x10
# define BLOCK_FACE_DOWN	0x20
# define BLOCK_SIZE 1

# define FRONT_COLOR_FACTOR .787f
# define BACK_COLOR_FACTOR .787f
# define LEFT_COLOR_FACTOR .59f
# define RIGHT_COLOR_FACTOR .59f
# define UP_COLOR_FACTOR .985f
# define DOWN_COLOR_FACTOR .496f

# define F1P1 0
# define F1P2 1
# define F1P3 2
# define F1P4 3
# define F2P1 4
# define F2P2 5
# define F2P3 6
# define F2P4 7
# define F3P1 8
# define F3P2 9
# define F3P3 10
# define F3P4 11
# define F4P1 12
# define F4P2 13
# define F4P3 14
# define F4P4 15
# define F5P1 16
# define F5P2 17
# define F5P3 18
# define F5P4 19
# define F6P1 20
# define F6P2 21
# define F6P3 22
# define F6P4 23

namespace voxel
{

	class Chunk;

	struct BlockLightsLevels
	{
		uint8_t f1p1 : 4;
		uint8_t f1p2 : 4;
		uint8_t f1p3 : 4;
		uint8_t f1p4 : 4;
		uint8_t f2p1 : 4;
		uint8_t f2p2 : 4;
		uint8_t f2p3 : 4;
		uint8_t f2p4 : 4;
		uint8_t f3p1 : 4;
		uint8_t f3p2 : 4;
		uint8_t f3p3 : 4;
		uint8_t f3p4 : 4;
		uint8_t f4p1 : 4;
		uint8_t f4p2 : 4;
		uint8_t f4p3 : 4;
		uint8_t f4p4 : 4;
		uint8_t f5p1 : 4;
		uint8_t f5p2 : 4;
		uint8_t f5p3 : 4;
		uint8_t f5p4 : 4;
		uint8_t f6p1 : 4;
		uint8_t f6p2 : 4;
		uint8_t f6p3 : 4;
		uint8_t f6p4 : 4;
	};

	class ChunkBlock
	{

	private:
		uint8_t type;
		bool shouldRenderFaceNear(ChunkBlock *block);
		bool calcTransparent(Chunk *chunk, Vec3 &pos, int8_t x, int8_t y, int8_t z);
		uint8_t calcLightLevel(Chunk *chunk, Vec3 &pos, int8_t x, int8_t y, int8_t z);
		void calcAmbientOcclusion(Vec3 &pos, BlockLightsLevels &lights, uint8_t visibleFaces, bool *blocksTransparent);
		void calcVisibleFaces(Chunk *chunk, Vec3 &pos, uint8_t &visibleFaces);
		void smoothLights(float *lights, uint8_t visibleFaces, BlockLightsLevels &lightsLevels, bool *blocksTransparent, int8_t *blockLights);
		void initLightsLevels(BlockLightsLevels &levels, uint8_t visibleFaces, int8_t *blocksLights);

	public:
		void fillBuffers(Chunk *chunk, Vec3 &pos, ChunkTessellator &tessellator, uint8_t layer);
		bool isTransparent();
		inline void setType(uint8_t type) {this->type = type;};
		inline uint8_t getType() {return (this->type);};
		static float getLightValue(int8_t light);

	};

}

#endif
