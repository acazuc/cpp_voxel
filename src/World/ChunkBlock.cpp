#include "ChunkBlock.h"
#include "Chunk.h"
#include "Main.h"
#include <cstring>

#define LIGHT_MIN .125
#define LIGHT_RANGE .7
#define AO_FACTOR 1

#define LIGHT_AO_TEST(light, ambientOcclusion) if (light < ambientOcclusion) {light = 0;} else {light -= ambientOcclusion;}

namespace voxel
{

	static const float lightsLevelsValues[] = {.03518, .04398, .05497, .06871, .08589, .10737, .13421, .16777, .20971, .26214, .32768, .4096, .512, .64, .8, 1};
	//static float lightsLevelsValues[] = {.0625, .125, .1875, .25, .3125, .375, .4375, .5, .5625, .625, .6875, .75, .8125, .875, .9375, 1};
	static const float texSize = 1. / 16;

	void ChunkBlock::fillBuffers(Chunk *chunk, Vec3 &pos, ChunkTessellator &tessellator, uint8_t layer)
	{
		if (this->type == 0)
			return;
		Block *blockModel = Blocks::getBlock(this->type);
		if (!blockModel)
			return;
		if (blockModel->getLayer() != layer)
			return;
		uint8_t visibleFaces;
		calcVisibleFaces(chunk, pos, visibleFaces);
		if (!visibleFaces)
			return;
		bool blocksTransparent[27];
		uint8_t blocksLights[27];
		{
			int8_t i = 0;
			for (int8_t x = -1; x <= 1; ++x)
			{
				for (int8_t y = -1; y <= 1; ++y)
				{
					for (int8_t z = -1; z <= 1; ++z)
					{
						blocksTransparent[i] = calcTransparent(chunk, pos, x, y, z);
						blocksLights[i++] = calcLightLevel(chunk, pos, x, y, z);
					}
				}
			}
		}
		BlockLightsLevels lightsLevels;
		std::memset(&lightsLevels, 0, sizeof(lightsLevels));
		initLightsLevels(lightsLevels, visibleFaces, blocksLights);
		if (Main::getSsao() && blockModel->isSolid())
			calcAmbientOcclusion(pos, lightsLevels, visibleFaces, blocksTransparent);
		Vec3 color(1, 1, 1);
		/*uint8_t biome = chunk->getBiome(pos.x - chunk->getX(), pos.z - chunk->getZ());
		color.x = (biome & 0x3) / 3.;
		color.y = ((biome & 0xf) >> 2) / 3.;
		color.z = ((biome & 0x3f) >> 4) / 3.;*/
		float lights[24];
		smoothLights(lights, visibleFaces, lightsLevels, blocksTransparent, blocksLights);
		blockModel->draw(chunk, pos, tessellator, visibleFaces, lights);
	}

	bool ChunkBlock::shouldRenderFaceNear(ChunkBlock *block)
	{
		if (!block)
			return true;
		if (!block->isTransparent())
			return false;
		if (block->getType() == this->type)
		{
			Block *blockModel = Blocks::getBlock(this->type);
			if (!blockModel || !blockModel->isRenderSameNeighbor())
				return false;
		}
		return true;
	}

	void ChunkBlock::calcVisibleFaces(Chunk *chunk, Vec3 &pos, uint8_t &visibleFaces)
	{
		visibleFaces = 0;
		if (pos.z - chunk->getZ() == CHUNK_WIDTH - 1)
		{
			if (!chunk->getChunkZMore())
				visibleFaces |= BLOCK_FACE_FRONT;
			else if (shouldRenderFaceNear(chunk->getChunkZMore()->getBlock(pos.x - chunk->getX(), pos.y, 0)))
				visibleFaces |= BLOCK_FACE_FRONT;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() + 1)))
		{
			visibleFaces |= BLOCK_FACE_FRONT;
		}
		if (pos.z - chunk->getZ() == 0)
		{
			if (!chunk->getChunkZLess())
				visibleFaces |= BLOCK_FACE_BACK;
			else if (shouldRenderFaceNear(chunk->getChunkZLess()->getBlock(pos.x - chunk->getX(), pos.y, CHUNK_WIDTH - 1)))
				visibleFaces |= BLOCK_FACE_BACK;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() - 1)))
		{
			visibleFaces |= BLOCK_FACE_BACK;
		}
		if (pos.x - chunk->getX() == 0)
		{
			if (!chunk->getChunkXLess())
				visibleFaces |= BLOCK_FACE_LEFT;
			else if (shouldRenderFaceNear(chunk->getChunkXLess()->getBlock(CHUNK_WIDTH - 1, pos.y, pos.z - chunk->getZ())))
				visibleFaces |= BLOCK_FACE_LEFT;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(pos.x - chunk->getX() - 1, pos.y, pos.z - chunk->getZ())))
		{
			visibleFaces |= BLOCK_FACE_LEFT;
		}
		if (pos.x - chunk->getX() == CHUNK_WIDTH - 1)
		{
			if (!chunk->getChunkXMore())
				visibleFaces |= BLOCK_FACE_RIGHT;
			else if (shouldRenderFaceNear(chunk->getChunkXMore()->getBlock(0, pos.y, pos.z - chunk->getZ())))
				visibleFaces |= BLOCK_FACE_RIGHT;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(pos.x - chunk->getX() + 1, pos.y, pos.z - chunk->getZ())))
		{
			visibleFaces |= BLOCK_FACE_RIGHT;
		}
		if (pos.y == CHUNK_HEIGHT - 1)
			visibleFaces |= BLOCK_FACE_UP;
		else if (shouldRenderFaceNear(chunk->getBlock(pos.x - chunk->getX(), pos.y + 1, pos.z - chunk->getZ())))
			visibleFaces |= BLOCK_FACE_UP;
		if (pos.y == 0)
			visibleFaces |= BLOCK_FACE_DOWN;
		else if (shouldRenderFaceNear(chunk->getBlock(pos.x - chunk->getX(), pos.y - 1, pos.z - chunk->getZ())))
			visibleFaces |= BLOCK_FACE_DOWN;
	}

	uint8_t ChunkBlock::calcLightLevel(Chunk *chunk, Vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return 15;
		int32_t newX = pos.x - chunk->getX() + addX;
		int32_t newY = pos.y + addY;
		int32_t newZ = pos.z - chunk->getZ() + addZ;
		if (newX < 0)
		{
			newX += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXLess()))
				return 15;
		}
		else if (newX >= CHUNK_WIDTH)
		{
			newX -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXMore()))
				return 15;
		}
		if (newZ < 0)
		{
			newZ += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZLess()))
				return 15;
		}
		else if (newZ >= CHUNK_WIDTH)
		{
			newZ -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZMore()))
				return 15;
		}
		return chunk->getLight(newX, newY, newZ);
	}

	bool ChunkBlock::calcTransparent(Chunk *chunk, Vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return true;
		int32_t newX = pos.x - chunk->getX() + addX;
		int32_t newY = pos.y + addY;
		int32_t newZ = pos.z - chunk->getZ() + addZ;
		if (newX < 0)
		{
			newX += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXLess()))
				return true;
		}
		else if (newX >= CHUNK_WIDTH)
		{
			newX -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXMore()))
				return true;
		}
		if (newZ < 0)
		{
			newZ += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZLess()))
				return true;
		}
		else if (newZ >= CHUNK_WIDTH)
		{
			newZ -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZMore()))
				return true;
		}
		ChunkBlock *block = chunk->getBlock(newX, newY, newZ);
		if (!block)
			return true;
		if (!Blocks::getBlock(block->getType()))
			return true;
		return block->isTransparent();
	}

	void ChunkBlock::initLightsLevels(BlockLightsLevels &lights, uint8_t visibleFaces, uint8_t *blocksLights)
	{
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			uint8_t value = blocksLights[(1 * 3 + 1) * 3 + 2];
			lights.f1p1 = value;
			lights.f1p2 = value;
			lights.f1p3 = value;
			lights.f1p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			uint8_t value = blocksLights[(1 * 3 + 1) * 3 + 0];
			lights.f2p1 = value;
			lights.f2p2 = value;
			lights.f2p3 = value;
			lights.f2p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			uint8_t value = blocksLights[(0 * 3 + 1) * 3 + 1];
			lights.f3p1 = value;
			lights.f3p2 = value;
			lights.f3p3 = value;
			lights.f3p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			uint8_t value = blocksLights[(2 * 3 + 1) * 3 + 1];
			lights.f4p1 = value;
			lights.f4p2 = value;
			lights.f4p3 = value;
			lights.f4p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			uint8_t value = blocksLights[(1 * 3 + 2) * 3 + 1];
			lights.f5p1 = value;
			lights.f5p2 = value;
			lights.f5p3 = value;
			lights.f5p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			uint8_t value = blocksLights[(1 * 3 + 0) * 3 + 1];
			lights.f6p1 = value;
			lights.f6p2 = value;
			lights.f6p3 = value;
			lights.f6p4 = value;
		}
	}

	void ChunkBlock::calcAmbientOcclusion(Vec3 &pos, BlockLightsLevels &lights, uint8_t visibleFaces, bool *blocksTransparent)
	{
		BlockLightsLevels ambientOcclusion;
		std::memset(&ambientOcclusion, 0x00, sizeof(ambientOcclusion));
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ambientOcclusion.f1p4 += AO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 2])
			{
				ambientOcclusion.f1p4 += AO_FACTOR;
				ambientOcclusion.f1p1 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ambientOcclusion.f1p1 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ambientOcclusion.f1p2 += AO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 2])
			{
				ambientOcclusion.f1p2 += AO_FACTOR;
				ambientOcclusion.f1p3 += AO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ambientOcclusion.f1p3 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 2])
			{
				ambientOcclusion.f1p3 += AO_FACTOR;
				ambientOcclusion.f1p4 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 2])
			{
				ambientOcclusion.f1p1 += AO_FACTOR;
				ambientOcclusion.f1p2 += AO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ambientOcclusion.f2p4 += AO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 0])
			{
				ambientOcclusion.f2p4 += AO_FACTOR;
				ambientOcclusion.f2p1 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ambientOcclusion.f2p1 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ambientOcclusion.f2p2 += AO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 0])
			{
				ambientOcclusion.f2p2 += AO_FACTOR;
				ambientOcclusion.f2p3 += AO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ambientOcclusion.f2p3 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 0])
			{
				ambientOcclusion.f2p3 += AO_FACTOR;
				ambientOcclusion.f2p4 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 0])
			{
				ambientOcclusion.f2p1 += AO_FACTOR;
				ambientOcclusion.f2p2 += AO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ambientOcclusion.f3p4 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 1])
			{
				ambientOcclusion.f3p4 += AO_FACTOR;
				ambientOcclusion.f3p1 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ambientOcclusion.f3p1 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ambientOcclusion.f3p2 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 1])
			{
				ambientOcclusion.f3p2 += AO_FACTOR;
				ambientOcclusion.f3p3 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ambientOcclusion.f3p3 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 2])
			{
				ambientOcclusion.f3p3 += AO_FACTOR;
				ambientOcclusion.f3p4 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 0])
			{
				ambientOcclusion.f3p1 += AO_FACTOR;
				ambientOcclusion.f3p2 += AO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ambientOcclusion.f4p4 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 1])
			{
				ambientOcclusion.f4p4 += AO_FACTOR;
				ambientOcclusion.f4p1 += AO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ambientOcclusion.f4p1 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ambientOcclusion.f4p2 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 1])
			{
				ambientOcclusion.f4p2 += AO_FACTOR;
				ambientOcclusion.f4p3 += AO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ambientOcclusion.f4p3 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 2])
			{
				ambientOcclusion.f4p3 += AO_FACTOR;
				ambientOcclusion.f4p4 += AO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 0])
			{
				ambientOcclusion.f4p1 += AO_FACTOR;
				ambientOcclusion.f4p2 += AO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_UP && pos.y < CHUNK_HEIGHT - 1)
		{
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ambientOcclusion.f5p2 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 1])
			{
				ambientOcclusion.f5p1 += AO_FACTOR;
				ambientOcclusion.f5p2 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ambientOcclusion.f5p1 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ambientOcclusion.f5p3 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 1])
			{
				ambientOcclusion.f5p3 += AO_FACTOR;
				ambientOcclusion.f5p4 += AO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ambientOcclusion.f5p4 += AO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 0])
			{
				ambientOcclusion.f5p2 += AO_FACTOR;
				ambientOcclusion.f5p3 += AO_FACTOR;
			}
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 2])
			{
				ambientOcclusion.f5p4 += AO_FACTOR;
				ambientOcclusion.f5p1 += AO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN && pos.y > 0)
		{
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ambientOcclusion.f6p1 += AO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 1])
			{
				ambientOcclusion.f6p1 += AO_FACTOR;
				ambientOcclusion.f6p2 += AO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ambientOcclusion.f6p2 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ambientOcclusion.f6p3 += AO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 1])
			{
				ambientOcclusion.f6p3 += AO_FACTOR;
				ambientOcclusion.f6p4 += AO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ambientOcclusion.f6p4 += AO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 0])
			{
				ambientOcclusion.f6p2 += AO_FACTOR;
				ambientOcclusion.f6p3 += AO_FACTOR;
			}
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 2])
			{
				ambientOcclusion.f6p4 += AO_FACTOR;
				ambientOcclusion.f6p1 += AO_FACTOR;
			}
		}
		LIGHT_AO_TEST(lights.f1p1, ambientOcclusion.f1p1);
		LIGHT_AO_TEST(lights.f1p2, ambientOcclusion.f1p2);
		LIGHT_AO_TEST(lights.f1p3, ambientOcclusion.f1p3);
		LIGHT_AO_TEST(lights.f1p4, ambientOcclusion.f1p4);
		LIGHT_AO_TEST(lights.f2p1, ambientOcclusion.f2p1);
		LIGHT_AO_TEST(lights.f2p2, ambientOcclusion.f2p2);
		LIGHT_AO_TEST(lights.f2p3, ambientOcclusion.f2p3);
		LIGHT_AO_TEST(lights.f2p4, ambientOcclusion.f2p4);
		LIGHT_AO_TEST(lights.f3p1, ambientOcclusion.f3p1);
		LIGHT_AO_TEST(lights.f3p2, ambientOcclusion.f3p2);
		LIGHT_AO_TEST(lights.f3p3, ambientOcclusion.f3p3);
		LIGHT_AO_TEST(lights.f3p4, ambientOcclusion.f3p4);
		LIGHT_AO_TEST(lights.f4p1, ambientOcclusion.f4p1);
		LIGHT_AO_TEST(lights.f4p2, ambientOcclusion.f4p2);
		LIGHT_AO_TEST(lights.f4p3, ambientOcclusion.f4p3);
		LIGHT_AO_TEST(lights.f4p4, ambientOcclusion.f4p4);
		LIGHT_AO_TEST(lights.f5p1, ambientOcclusion.f5p1);
		LIGHT_AO_TEST(lights.f5p2, ambientOcclusion.f5p2);
		LIGHT_AO_TEST(lights.f5p3, ambientOcclusion.f5p3);
		LIGHT_AO_TEST(lights.f5p4, ambientOcclusion.f5p4);
		LIGHT_AO_TEST(lights.f6p1, ambientOcclusion.f6p1);
		LIGHT_AO_TEST(lights.f6p2, ambientOcclusion.f6p2);
		LIGHT_AO_TEST(lights.f6p3, ambientOcclusion.f6p3);
		LIGHT_AO_TEST(lights.f6p4, ambientOcclusion.f6p4);
	}

	void ChunkBlock::smoothLights(float *lights, uint8_t visibleFaces, BlockLightsLevels &lightsLevels, bool *blocksTransparent, uint8_t *blocksLights)
	{
		Block *blockModel = Blocks::getBlock(this->type);
		if (!Main::getSmooth() || !blockModel->isFullCube() || isTransparent())
		{
			lights[F1P1] = getLightValue(lightsLevels.f1p1);
			lights[F1P2] = getLightValue(lightsLevels.f1p2);
			lights[F1P3] = getLightValue(lightsLevels.f1p3);
			lights[F1P4] = getLightValue(lightsLevels.f1p4);
			lights[F2P1] = getLightValue(lightsLevels.f2p1);
			lights[F2P2] = getLightValue(lightsLevels.f2p2);
			lights[F2P3] = getLightValue(lightsLevels.f2p3);
			lights[F2P4] = getLightValue(lightsLevels.f2p4);
			lights[F3P1] = getLightValue(lightsLevels.f3p1);
			lights[F3P2] = getLightValue(lightsLevels.f3p2);
			lights[F3P3] = getLightValue(lightsLevels.f3p3);
			lights[F3P4] = getLightValue(lightsLevels.f3p4);
			lights[F4P1] = getLightValue(lightsLevels.f4p1);
			lights[F4P2] = getLightValue(lightsLevels.f4p2);
			lights[F4P3] = getLightValue(lightsLevels.f4p3);
			lights[F4P4] = getLightValue(lightsLevels.f4p4);
			lights[F5P1] = getLightValue(lightsLevels.f5p1);
			lights[F5P2] = getLightValue(lightsLevels.f5p2);
			lights[F5P3] = getLightValue(lightsLevels.f5p3);
			lights[F5P4] = getLightValue(lightsLevels.f5p4);
			lights[F6P1] = getLightValue(lightsLevels.f6p1);
			lights[F6P2] = getLightValue(lightsLevels.f6p2);
			lights[F6P3] = getLightValue(lightsLevels.f6p3);
			lights[F6P4] = getLightValue(lightsLevels.f6p4);
			return;
		}
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 2];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F1P1] = getLightValue(average + lightsLevels.f1p1 - blocksLights[(1 * 3 + 1) * 3 + 2]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 2];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F1P4] = getLightValue(average + lightsLevels.f1p4 - blocksLights[(1 * 3 + 1) * 3 + 2]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 2];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F1P3] = getLightValue(average + lightsLevels.f1p3 - blocksLights[(1 * 3 + 1) * 3 + 2]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 2];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F1P2] = getLightValue(average + lightsLevels.f1p2 - blocksLights[(1 * 3 + 1) * 3 + 2]);
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 0];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F2P1] = getLightValue(average + lightsLevels.f2p1 - blocksLights[(1 * 3 + 1) * 3 + 0]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 0];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F2P4] = getLightValue(average + lightsLevels.f2p4 - blocksLights[(1 * 3 + 1) * 3 + 0]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 0];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F2P3] = getLightValue(average + lightsLevels.f2p3 - blocksLights[(1 * 3 + 1) * 3 + 0]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 0];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F2P2] = getLightValue(average + lightsLevels.f2p2 - blocksLights[(1 * 3 + 1) * 3 + 0]);
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 0; y <= 1; ++y)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(0 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F3P1] = getLightValue(average + lightsLevels.f3p1 - blocksLights[(0 * 3 + 1) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 1; y <= 2; ++y)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(0 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F3P2] = getLightValue(average + lightsLevels.f3p2 - blocksLights[(0 * 3 + 1) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 1; y <= 2; ++y)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(0 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F3P3] = getLightValue(average + lightsLevels.f3p3 - blocksLights[(0 * 3 + 1) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 0; y <= 1; ++y)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(0 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F3P4] = getLightValue(average + lightsLevels.f3p4 - blocksLights[(0 * 3 + 1) * 3 + 1]);
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 0; y <= 1; ++y)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(2 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F4P1] = getLightValue(average + lightsLevels.f4p1 - blocksLights[(2 * 3 + 1) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 1; y <= 2; ++y)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(2 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F4P2] = getLightValue(average + lightsLevels.f4p2 - blocksLights[(2 * 3 + 1) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 1; y <= 2; ++y)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(2 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F4P3] = getLightValue(average + lightsLevels.f4p3 - blocksLights[(2 * 3 + 1) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t y = 0; y <= 1; ++y)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(2 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F4P4] = getLightValue(average + lightsLevels.f4p4 - blocksLights[(2 * 3 + 1) * 3 + 1]);
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F5P2] = getLightValue(average + lightsLevels.f5p2 - blocksLights[(1 * 3 + 2) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F5P3] = getLightValue(average + lightsLevels.f5p3 - blocksLights[(1 * 3 + 2) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F5P4] = getLightValue(average + lightsLevels.f5p4 - blocksLights[(1 * 3 + 2) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F5P1] = getLightValue(average + lightsLevels.f5p1 - blocksLights[(1 * 3 + 2) * 3 + 1]);
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 0) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F6P2] = getLightValue(average + lightsLevels.f6p2 - blocksLights[(1 * 3 + 0) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 0) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F6P3] = getLightValue(average + lightsLevels.f6p3 - blocksLights[(1 * 3 + 0) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 1; x <= 2; ++x)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 0) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F6P4] = getLightValue(average + lightsLevels.f6p4 - blocksLights[(1 * 3 + 0) * 3 + 1]);
			}
			{
				uint8_t totalLight = 0;
				uint8_t lightsPoints = 0;
				for (uint8_t x = 0; x <= 1; ++x)
				{
					for (uint8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 0) * 3 + z];
							lightsPoints++;
						}
					}
				}
				uint8_t average = totalLight / lightsPoints;
				lights[F6P1] = getLightValue(average + lightsLevels.f6p1 - blocksLights[(1 * 3 + 0) * 3 + 1]);
			}
		}
	}

	float ChunkBlock::getLightValue(uint8_t level)
	{
		level = std::max((uint8_t)0, std::min((uint8_t)15, level));
		return lightsLevelsValues[level] * LIGHT_RANGE + LIGHT_MIN;
	}

	bool ChunkBlock::isTransparent()
	{
		if (this->type == 0)
			return true;
		Block *block = Blocks::getBlock(this->type);
		if (!block)
			return true;
		if (block->isTransparent())
			return true;
		return false;
	}

}
