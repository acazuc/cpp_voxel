#ifndef CHUNK_BLOCK_H
# define CHUNK_BLOCK_H

# include "ChunkTessellator.h"
# include "Blocks/Blocks.h"
# include "AABB.h"
# include <librender/GL.h>
# include <cstdint>
# include <vector>

# define BLOCK_SIZE 1

# define FRONT_COLOR_FACTOR .787f
# define BACK_COLOR_FACTOR .787f
# define LEFT_COLOR_FACTOR .59f
# define RIGHT_COLOR_FACTOR .59f
# define UP_COLOR_FACTOR .985f
# define DOWN_COLOR_FACTOR .496f

namespace voxel
{

	enum BlockFaces
	{
		BLOCK_FACE_FRONT =	0x01,
		BLOCK_FACE_BACK =	0x02,
		BLOCK_FACE_LEFT =	0x04,
		BLOCK_FACE_RIGHT =	0x08,
		BLOCK_FACE_UP =		0x10,
		BLOCK_FACE_DOWN =	0x20
	};

	enum BlockFacePoints
	{
		F1P1,
		F1P2,
		F1P3,
		F1P4,
		F2P1,
		F2P2,
		F2P3,
		F2P4,
		F3P1,
		F3P2,
		F3P3,
		F3P4,
		F4P1,
		F4P2,
		F4P3,
		F4P4,
		F5P1,
		F5P2,
		F5P3,
		F5P4,
		F6P1,
		F6P2,
		F6P3,
		F6P4,
	};

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
		void smoothLights(float *lights, uint8_t visibleFaces, BlockLightsLevels &lightsLevels, bool *blocksTransparent, uint8_t *blockLights);
		void initLightsLevels(BlockLightsLevels &levels, uint8_t visibleFaces, uint8_t *blocksLights);

	public:
		void fillBuffers(Chunk *chunk, Vec3 &pos, ChunkTessellator &tessellator, uint8_t layer);
		bool isTransparent();
		inline void setType(uint8_t type) {this->type = type;};
		inline uint8_t getType() {return this->type;};
		static float getLightValue(uint8_t light);

	};

}

#endif
