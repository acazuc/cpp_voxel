#include "ChunkBlock.h"
#include "Blocks/Blocks.h"
#include "Chunk.h"
#include "Main.h"
#include <cstring>

#define LIGHT_MIN .125
#define LIGHT_RANGE .7
#define SSAO_FACTOR 2

#define F1P1 0
#define F1P2 1
#define F1P3 2
#define F1P4 3
#define F2P1 4
#define F2P2 5
#define F2P3 6
#define F2P4 7
#define F3P1 8
#define F3P2 9
#define F3P3 10
#define F3P4 11
#define F4P1 12
#define F4P2 13
#define F4P3 14
#define F4P4 15
#define F5P1 16
#define F5P2 17
#define F5P3 18
#define F5P4 19
#define F6P1 20
#define F6P2 21
#define F6P3 22
#define F6P4 23

#define LIGHT_SSAO_TEST(light, ssao) if (light <= ssao) {light = 0;} else {light -= ssao;}

namespace voxel
{

	static const float lightsLevelsValues[] = {.03518, .04398, .05497, .06871, .08589, .10737, .13421, .16777, .20971, .26214, .32768, .4096, .512, .64, .8, 1};
	//static float lightsLevelsValues[] = {.0625, .125, .1875, .25, .3125, .375, .4375, .5, .5625, .625, .6875, .75, .8125, .875, .9375, 1};
	static const float texSize = 1. / 16;

	ChunkBlock::ChunkBlock(uint8_t type)
	: type(type)
	{
		//Empty
	}

	ChunkBlock::ChunkBlock()
	: type(0)
	{
		//Empty
	}

	void ChunkBlock::fillBuffers(Chunk *chunk, glm::vec3 &pos, std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices)
	{
		if (this->type == 0)
			return;
		uint8_t visibleFaces;
		calcVisibleFaces(chunk, pos, visibleFaces);
		if (!visibleFaces)
			return;
		bool blocksTransparent[27];
		int8_t blocksLights[27];
		{
			int8_t i = 0;
			for (int8_t x = -1; x <= 1; ++x)
			{
				for (int8_t y = -1; y <= 1; ++y)
				{
					for (int8_t z = -1; z <= 1; ++z)
					{
						blocksTransparent[i] = calcLightsLevelsIsTransparent(chunk, pos, x, y, z);
						blocksLights[i++] = calcLightLevel(chunk, pos, x, y, z);
					}
				}
			}
		}
		BlockLightsLevels lightsLevels;
		initLightsLevels(lightsLevels, visibleFaces, blocksLights);
		std::memset(&lightsLevels, ((blocksLights[(1 * 3 + 1) * 3 + 1] & 0xf) << 4) | (blocksLights[(1 * 3 + 1) * 3 + 1] & 0xf), sizeof(lightsLevels));
		if (Main::getSsao())
			calcAmbientOcclusion(pos, lightsLevels, visibleFaces, blocksTransparent);
		glm::vec3 color(1, 1, 1);
		float lights[24];
		smoothLights(lights, visibleFaces, lightsLevels, blocksTransparent, blocksLights);
		Block *blockModel = Blocks::getBlock(this->type);
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x, pos.y, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexFrontX(), blockModel->getTexFrontY() + texSize));
			colors.push_back(glm::vec3(lights[F1P1] * color.x, lights[F1P1] * color.y, lights[F1P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexFrontX(), blockModel->getTexFrontY()));
			colors.push_back(glm::vec3(lights[F1P2] * color.x, lights[F1P2] * color.y, lights[F1P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexFrontX() + texSize, blockModel->getTexFrontY()));
			colors.push_back(glm::vec3(lights[F1P3] * color.x, lights[F1P3] * color.y, lights[F1P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexFrontX() + texSize, blockModel->getTexFrontY() + texSize));
			colors.push_back(glm::vec3(lights[F1P4] * color.x, lights[F1P4] * color.y, lights[F1P4] * color.z));
			if (lightsLevels.f1p2 + lightsLevels.f1p4 > lightsLevels.f1p1 + lightsLevels.f1p3)
			{
				indices.push_back(currentIndice + 1); //1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); // 3
				indices.push_back(currentIndice + 3);
			}
			else
			{
				indices.push_back(currentIndice + 2); // 1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); //3
				indices.push_back(currentIndice + 0);
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x, pos.y, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexBackX() + texSize, blockModel->getTexBackY() + texSize));
			colors.push_back(glm::vec3(lights[F2P1] * color.x, lights[F2P1] * color.y, lights[F2P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x, pos.y + BLOCK_SIZE, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexBackX() + texSize, blockModel->getTexBackY()));
			colors.push_back(glm::vec3(lights[F2P2] * color.x, lights[F2P2] * color.y, lights[F2P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexBackX(), blockModel->getTexBackY()));
			colors.push_back(glm::vec3(lights[F2P3] * color.x, lights[F2P3] * color.y, lights[F2P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexBackX(), blockModel->getTexBackY() + texSize));
			colors.push_back(glm::vec3(lights[F2P4] * color.x, lights[F2P4] * color.y, lights[F2P4] * color.z));
			if (lightsLevels.f2p2 + lightsLevels.f2p4 <= lightsLevels.f2p1 + lightsLevels.f2p3)
			{
				indices.push_back(currentIndice + 2); //1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); // 3
				indices.push_back(currentIndice + 0);
			}
			else
			{
				indices.push_back(currentIndice + 1); // 1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); //3
				indices.push_back(currentIndice + 3);
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x, pos.y, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexLeftX(), blockModel->getTexLeftY() + texSize));
			colors.push_back(glm::vec3(lights[F3P1] * color.x, lights[F3P1] * color.y, lights[F3P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x, pos.y + BLOCK_SIZE, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexLeftX(), blockModel->getTexLeftY()));
			colors.push_back(glm::vec3(lights[F3P2] * color.x, lights[F3P2] * color.y, lights[F3P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexLeftX() + texSize, blockModel->getTexLeftY()));
			colors.push_back(glm::vec3(lights[F3P3] * color.x, lights[F3P3] * color.y, lights[F3P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x, pos.y, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexLeftX() + texSize, blockModel->getTexLeftY() + texSize));
			colors.push_back(glm::vec3(lights[F3P4] * color.x, lights[F3P4] * color.y, lights[F3P4] * color.z));
			if (lightsLevels.f3p2 + lightsLevels.f3p4 > lightsLevels.f3p1 + lightsLevels.f3p3)
			{
				indices.push_back(currentIndice + 1); //1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); // 3
				indices.push_back(currentIndice + 3);
			}
			else
			{
				indices.push_back(currentIndice + 2); // 1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); //3
				indices.push_back(currentIndice + 0);
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexRightX() + texSize, blockModel->getTexRightY() + texSize));
			colors.push_back(glm::vec3(lights[F4P1] * color.x, lights[F4P1] * color.y, lights[F4P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexRightX() + texSize, blockModel->getTexRightY()));
			colors.push_back(glm::vec3(lights[F4P2] * color.x, lights[F4P2] * color.y, lights[F4P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexRightX(), blockModel->getTexRightY()));
			colors.push_back(glm::vec3(lights[F4P3] * color.x, lights[F4P3] * color.y, lights[F4P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexRightX(), blockModel->getTexRightY() + texSize));
			colors.push_back(glm::vec3(lights[F4P4] * color.x, lights[F4P4] * color.y, lights[F4P4] * color.z));
			if (lightsLevels.f4p2 + lightsLevels.f4p4 < lightsLevels.f4p1 + lightsLevels.f4p3)
			{
				indices.push_back(currentIndice + 2); //1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); // 3
				indices.push_back(currentIndice + 0);
			}
			else
			{
				indices.push_back(currentIndice + 1); // 1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); //3
				indices.push_back(currentIndice + 3);
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexTopX(), blockModel->getTexTopY()));
			colors.push_back(glm::vec3(lights[F5P1] * color.x, lights[F5P1] * color.y, lights[F5P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x, pos.y + BLOCK_SIZE, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexTopX(), blockModel->getTexTopY() + texSize));
			colors.push_back(glm::vec3(lights[F5P2] * color.x, lights[F5P2] * color.y, lights[F5P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexTopX() + texSize, blockModel->getTexTopY() + texSize));
			colors.push_back(glm::vec3(lights[F5P3] * color.x, lights[F5P3] * color.y, lights[F5P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexTopX() + texSize, blockModel->getTexTopY()));
			colors.push_back(glm::vec3(lights[F5P4] * color.x, lights[F5P4] * color.y, lights[F5P4] * color.z));
			if (lightsLevels.f5p2 + lightsLevels.f5p4 > lightsLevels.f5p1 + lightsLevels.f5p3)
			{
				indices.push_back(currentIndice + 1); //1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); // 3
				indices.push_back(currentIndice + 3);
			}
			else
			{
				indices.push_back(currentIndice + 2); // 1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); //3
				indices.push_back(currentIndice + 0);
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x, pos.y, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexBotX(), blockModel->getTexBotY()));
			colors.push_back(glm::vec3(lights[F6P1] * color.x, lights[F6P1] * color.y, lights[F6P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x, pos.y, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexBotX(), blockModel->getTexBotY() + texSize));
			colors.push_back(glm::vec3(lights[F6P2] * color.x, lights[F6P2] * color.y, lights[F6P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y, pos.z));
			texCoords.push_back(glm::vec2(blockModel->getTexBotX() + texSize, blockModel->getTexBotY() + texSize));
			colors.push_back(glm::vec3(lights[F6P3] * color.x, lights[F6P3] * color.y, lights[F6P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(blockModel->getTexBotX() + texSize, blockModel->getTexBotY()));
			colors.push_back(glm::vec3(lights[F6P4] * color.x, lights[F6P4] * color.y, lights[F6P4] * color.z));
			if (lightsLevels.f6p2 + lightsLevels.f6p4 < lightsLevels.f6p1 + lightsLevels.f6p3)
			{
				indices.push_back(currentIndice + 0); //1
				indices.push_back(currentIndice + 1); //23
				indices.push_back(currentIndice + 2);
				indices.push_back(currentIndice + 3); //21
				indices.push_back(currentIndice + 0); // 3
				indices.push_back(currentIndice + 2);
			}
			else
			{
				indices.push_back(currentIndice + 3); // 1
				indices.push_back(currentIndice + 1); //23
				indices.push_back(currentIndice + 2);
				indices.push_back(currentIndice + 3); //21
				indices.push_back(currentIndice + 0); //3
				indices.push_back(currentIndice + 1);
			}
		}
	}

	void ChunkBlock::calcVisibleFaces(Chunk *chunk, glm::vec3 &pos, uint8_t &visibleFaces)
	{
		visibleFaces = 0;
		if (pos.z - chunk->getZ() == CHUNK_WIDTH - 1)
		{
			if (!chunk->getChunkZMore())
			{
				visibleFaces |= BLOCK_FACE_FRONT;
			}
			else
			{
				ChunkBlock *block = chunk->getChunkZMore()->getBlockAt(pos.x - chunk->getX(), pos.y, 0);
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_FRONT;
			}
		}
		else
		{
			ChunkBlock *block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() + 1);
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_FRONT;
		}
		if (pos.z - chunk->getZ() == 0)
		{
			if (!chunk->getChunkZLess())
			{
				visibleFaces |= BLOCK_FACE_BACK;
			}
			else
			{
				ChunkBlock *block = chunk->getChunkZLess()->getBlockAt(pos.x - chunk->getX(), pos.y, CHUNK_WIDTH - 1);
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_BACK;
			}
		}
		else
		{
			ChunkBlock *block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() - 1);
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_BACK;
		}
		if (pos.x - chunk->getX() == 0)
		{
			if (!chunk->getChunkXLess())
			{
				visibleFaces |= BLOCK_FACE_LEFT;
			}
			else
			{
				ChunkBlock *block = chunk->getChunkXLess()->getBlockAt(CHUNK_WIDTH - 1, pos.y, pos.z - chunk->getZ());
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_LEFT;
			}
		}
		else
		{
			ChunkBlock *block = chunk->getBlockAt(pos.x - chunk->getX() - 1, pos.y, pos.z - chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_LEFT;
		}
		if (pos.x - chunk->getX() == CHUNK_WIDTH - 1)
		{
			if (!chunk->getChunkXMore())
			{
				visibleFaces |= BLOCK_FACE_RIGHT;
			}
			else
			{
				ChunkBlock *block = chunk->getChunkXMore()->getBlockAt(0, pos.y, pos.z - chunk->getZ());
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_RIGHT;
			}
		}
		else
		{
			ChunkBlock *block = chunk->getBlockAt(pos.x - chunk->getX() + 1, pos.y, pos.z - chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_RIGHT;
		}
		if (pos.y == CHUNK_HEIGHT - 1)
		{
			visibleFaces |= BLOCK_FACE_UP;
		}
		else
		{
			ChunkBlock *block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y + 1, pos.z - chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_UP;
		}
		if (pos.y == 0)
		{
			visibleFaces |= BLOCK_FACE_DOWN;
		}
		else
		{
			ChunkBlock * block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y - 1, pos.z - chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_DOWN;
		}
	}

	uint8_t ChunkBlock::calcLightLevel(Chunk *chunk, glm::vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return (15);
		int32_t newX = pos.x - chunk->getX() + addX;
		int32_t newY = pos.y + addY;
		int32_t newZ = pos.z - chunk->getZ() + addZ;
		if (newX < 0)
		{
			newX += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXLess()))
				return (15);
		}
		else if (newX >= CHUNK_WIDTH)
		{
			newX -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXMore()))
				return (15);
		}
		if (newZ < 0)
		{
			newZ += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZLess()))
				return (15);
		}
		else if (newZ >= CHUNK_WIDTH)
		{
			newZ -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZMore()))
				return (15);
		}
		return (chunk->getLightAt(newX, newY, newZ));
	}

	bool ChunkBlock::calcLightsLevelsIsTransparent(Chunk *chunk, glm::vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return (true);
		int32_t newX = pos.x - chunk->getX() + addX;
		int32_t newY = pos.y + addY;
		int32_t newZ = pos.z - chunk->getZ() + addZ;
		if (newX < 0)
		{
			newX += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXLess()))
				return (true);
		}
		else if (newX >= CHUNK_WIDTH)
		{
			newX -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXMore()))
				return (true);
		}
		if (newZ < 0)
		{
			newZ += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZLess()))
				return (true);
		}
		else if (newZ >= CHUNK_WIDTH)
		{
			newZ -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZMore()))
				return (true);
		}
		ChunkBlock *block = chunk->getBlockAt(newX, newY, newZ);
		return (block->isTransparent());
	}

	void ChunkBlock::initLightsLevels(BlockLightsLevels &/*lights*/, uint8_t /*visibleFaces*/, int8_t * /*blocksLights*/)
	{
		/*if (visibleFaces & BLOCK_FACE_FRONT)
		{
			int8_t value = blocksLights[(1 * 3 + 1) * 3 + 2];
			lights.f1p1 = value;
			lights.f1p2 = value;
			lights.f1p3 = value;
			lights.f1p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			int8_t value = blocksLights[(1 * 3 + 1) * 3 + 0];
			lights.f2p1 = value;
			lights.f2p2 = value;
			lights.f2p3 = value;
			lights.f2p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			int8_t value = blocksLights[(0 * 3 + 1) * 3 + 1];
			lights.f3p1 = value;
			lights.f3p2 = value;
			lights.f3p3 = value;
			lights.f3p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			int8_t value = blocksLights[(2 * 3 + 1) * 3 + 1];
			lights.f4p1 = value;
			lights.f4p2 = value;
			lights.f4p3 = value;
			lights.f4p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			int8_t value = blocksLights[(1 * 3 + 2) * 3 + 1];
			lights.f5p1 = value;
			lights.f5p2 = value;
			lights.f5p3 = value;
			lights.f5p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			int8_t value = blocksLights[(1 * 3 + 0) * 3 + 1];
			lights.f6p1 = value;
			lights.f6p2 = value;
			lights.f6p3 = value;
			lights.f6p4 = value;
		}*/
	}

	void ChunkBlock::calcAmbientOcclusion(glm::vec3 &pos, BlockLightsLevels &lights, uint8_t visibleFaces, bool *blocksTransparent)
	{
		BlockLightsLevels ssao;
		std::memset(&ssao, 0x00, sizeof(ssao));
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ssao.f1p4 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 2])
			{
				ssao.f1p4 += SSAO_FACTOR;
				ssao.f1p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ssao.f1p1 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ssao.f1p2 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 2])
			{
				ssao.f1p2 += SSAO_FACTOR;
				ssao.f1p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ssao.f1p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 2])
			{
				ssao.f1p3 += SSAO_FACTOR;
				ssao.f1p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 2])
			{
				ssao.f1p1 += SSAO_FACTOR;
				ssao.f1p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ssao.f2p4 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 0])
			{
				ssao.f2p4 += SSAO_FACTOR;
				ssao.f2p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ssao.f2p1 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ssao.f2p2 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 0])
			{
				ssao.f2p2 += SSAO_FACTOR;
				ssao.f2p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ssao.f2p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 0])
			{
				ssao.f2p3 += SSAO_FACTOR;
				ssao.f2p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 0])
			{
				ssao.f2p1 += SSAO_FACTOR;
				ssao.f2p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ssao.f3p4 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 1])
			{
				ssao.f3p4 += SSAO_FACTOR;
				ssao.f3p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ssao.f3p1 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ssao.f3p2 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 1])
			{
				ssao.f3p2 += SSAO_FACTOR;
				ssao.f3p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ssao.f3p3 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 2])
			{
				ssao.f3p3 += SSAO_FACTOR;
				ssao.f3p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 0])
			{
				ssao.f3p1 += SSAO_FACTOR;
				ssao.f3p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ssao.f4p4 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 1])
			{
				ssao.f4p4 += SSAO_FACTOR;
				ssao.f4p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ssao.f4p1 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ssao.f4p2 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 1])
			{
				ssao.f4p2 += SSAO_FACTOR;
				ssao.f4p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ssao.f4p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 2])
			{
				ssao.f4p3 += SSAO_FACTOR;
				ssao.f4p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 0])
			{
				ssao.f4p1 += SSAO_FACTOR;
				ssao.f4p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			if (pos.y < CHUNK_HEIGHT - 1)
			{
				if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
					ssao.f5p2 += SSAO_FACTOR;
				if (!blocksTransparent[(0 * 3 + 2) * 3 + 1])
				{
					ssao.f5p1 += SSAO_FACTOR;
					ssao.f5p2 += SSAO_FACTOR;
				}
				if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
					ssao.f5p1 += SSAO_FACTOR;
				if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
					ssao.f5p3 += SSAO_FACTOR;
				if (!blocksTransparent[(2 * 3 + 2) * 3 + 1])
				{
					ssao.f5p3 += SSAO_FACTOR;
					ssao.f5p4 += SSAO_FACTOR;
				}
				if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
					ssao.f5p4 += SSAO_FACTOR;
				if (!blocksTransparent[(1 * 3 + 2) * 3 + 0])
				{
					ssao.f5p2 += SSAO_FACTOR;
					ssao.f5p3 += SSAO_FACTOR;
				}
				if (!blocksTransparent[(1 * 3 + 2) * 3 + 2])
				{
					ssao.f5p4 += SSAO_FACTOR;
					ssao.f5p1 += SSAO_FACTOR;
				}
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			if (pos.y > 0)
			{
				if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
					ssao.f6p1 += SSAO_FACTOR;
				if (!blocksTransparent[(0 * 3 + 0) * 3 + 1])
				{
					ssao.f6p1 += SSAO_FACTOR;
					ssao.f6p2 += SSAO_FACTOR;
				}
				if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
					ssao.f6p2 += SSAO_FACTOR;
				if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
					ssao.f6p3 += SSAO_FACTOR;
				if (!blocksTransparent[(2 * 3 + 0) * 3 + 1])
				{
					ssao.f6p3 += SSAO_FACTOR;
					ssao.f6p4 += SSAO_FACTOR;
				}
				if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
					ssao.f6p4 += SSAO_FACTOR;
				if (!blocksTransparent[(1 * 3 + 0) * 3 + 0])
				{
					ssao.f6p2 += SSAO_FACTOR;
					ssao.f6p3 += SSAO_FACTOR;
				}
				if (!blocksTransparent[(1 * 3 + 0) * 3 + 2])
				{
					ssao.f6p4 += SSAO_FACTOR;
					ssao.f6p1 += SSAO_FACTOR;
				}
			}
		}
		LIGHT_SSAO_TEST(lights.f1p1, ssao.f1p1);
		LIGHT_SSAO_TEST(lights.f1p2, ssao.f1p2);
		LIGHT_SSAO_TEST(lights.f1p3, ssao.f1p3);
		LIGHT_SSAO_TEST(lights.f1p4, ssao.f1p4);
		LIGHT_SSAO_TEST(lights.f2p1, ssao.f2p1);
		LIGHT_SSAO_TEST(lights.f2p2, ssao.f2p2);
		LIGHT_SSAO_TEST(lights.f2p3, ssao.f2p3);
		LIGHT_SSAO_TEST(lights.f2p4, ssao.f2p4);
		LIGHT_SSAO_TEST(lights.f3p1, ssao.f3p1);
		LIGHT_SSAO_TEST(lights.f3p2, ssao.f3p2);
		LIGHT_SSAO_TEST(lights.f3p3, ssao.f3p3);
		LIGHT_SSAO_TEST(lights.f3p4, ssao.f3p4);
		LIGHT_SSAO_TEST(lights.f4p1, ssao.f4p1);
		LIGHT_SSAO_TEST(lights.f4p2, ssao.f4p2);
		LIGHT_SSAO_TEST(lights.f4p3, ssao.f4p3);
		LIGHT_SSAO_TEST(lights.f4p4, ssao.f4p4);
		LIGHT_SSAO_TEST(lights.f5p1, ssao.f5p1);
		LIGHT_SSAO_TEST(lights.f5p2, ssao.f5p2);
		LIGHT_SSAO_TEST(lights.f5p3, ssao.f5p3);
		LIGHT_SSAO_TEST(lights.f5p4, ssao.f5p4);
		LIGHT_SSAO_TEST(lights.f6p1, ssao.f6p1);
		LIGHT_SSAO_TEST(lights.f6p2, ssao.f6p2);
		LIGHT_SSAO_TEST(lights.f6p3, ssao.f6p3);
		LIGHT_SSAO_TEST(lights.f6p4, ssao.f6p4);
	}

	void ChunkBlock::smoothLights(float *lights, uint8_t visibleFaces, BlockLightsLevels &lightsLevels, bool *blocksTransparent, int8_t *blocksLights)
	{
		if (!Main::getSmooth())
		{
			lights[F1P1] = lightsLevelsValues[lightsLevels.f1p1] * LIGHT_RANGE + LIGHT_MIN;
			lights[F1P2] = lightsLevelsValues[lightsLevels.f1p2] * LIGHT_RANGE + LIGHT_MIN;
			lights[F1P3] = lightsLevelsValues[lightsLevels.f1p3] * LIGHT_RANGE + LIGHT_MIN;
			lights[F1P4] = lightsLevelsValues[lightsLevels.f1p4] * LIGHT_RANGE + LIGHT_MIN;
			lights[F2P1] = lightsLevelsValues[lightsLevels.f2p1] * LIGHT_RANGE + LIGHT_MIN;
			lights[F2P2] = lightsLevelsValues[lightsLevels.f2p2] * LIGHT_RANGE + LIGHT_MIN;
			lights[F2P3] = lightsLevelsValues[lightsLevels.f2p3] * LIGHT_RANGE + LIGHT_MIN;
			lights[F2P4] = lightsLevelsValues[lightsLevels.f2p4] * LIGHT_RANGE + LIGHT_MIN;
			lights[F3P1] = lightsLevelsValues[lightsLevels.f3p1] * LIGHT_RANGE + LIGHT_MIN;
			lights[F3P2] = lightsLevelsValues[lightsLevels.f3p2] * LIGHT_RANGE + LIGHT_MIN;
			lights[F3P3] = lightsLevelsValues[lightsLevels.f3p3] * LIGHT_RANGE + LIGHT_MIN;
			lights[F3P4] = lightsLevelsValues[lightsLevels.f3p4] * LIGHT_RANGE + LIGHT_MIN;
			lights[F4P1] = lightsLevelsValues[lightsLevels.f4p1] * LIGHT_RANGE + LIGHT_MIN;
			lights[F4P2] = lightsLevelsValues[lightsLevels.f4p2] * LIGHT_RANGE + LIGHT_MIN;
			lights[F4P3] = lightsLevelsValues[lightsLevels.f4p3] * LIGHT_RANGE + LIGHT_MIN;
			lights[F4P4] = lightsLevelsValues[lightsLevels.f4p4] * LIGHT_RANGE + LIGHT_MIN;
			lights[F5P1] = lightsLevelsValues[lightsLevels.f5p1] * LIGHT_RANGE + LIGHT_MIN;
			lights[F5P2] = lightsLevelsValues[lightsLevels.f5p2] * LIGHT_RANGE + LIGHT_MIN;
			lights[F5P3] = lightsLevelsValues[lightsLevels.f5p3] * LIGHT_RANGE + LIGHT_MIN;
			lights[F5P4] = lightsLevelsValues[lightsLevels.f5p4] * LIGHT_RANGE + LIGHT_MIN;
			lights[F6P1] = lightsLevelsValues[lightsLevels.f6p1] * LIGHT_RANGE + LIGHT_MIN;
			lights[F6P2] = lightsLevelsValues[lightsLevels.f6p2] * LIGHT_RANGE + LIGHT_MIN;
			lights[F6P3] = lightsLevelsValues[lightsLevels.f6p3] * LIGHT_RANGE + LIGHT_MIN;
			lights[F6P4] = lightsLevelsValues[lightsLevels.f6p4] * LIGHT_RANGE + LIGHT_MIN;
			return;
		}
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f1p1 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F1P1] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f1p4 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F1P4] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f1p3 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F1P3] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f1p2 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F1P2] = LIGHT_MIN + result * LIGHT_RANGE;
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f2p1 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F2P1] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f2p4 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F2P4] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f2p3 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F2P3] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1])
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f2p2 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F2P2] = LIGHT_MIN + result * LIGHT_RANGE;
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f3p1 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F3P1] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f3p2 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F3P2] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f3p3 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F3P3] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f3p4 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F3P4] = LIGHT_MIN + result * LIGHT_RANGE;
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f4p1 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F4P1] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f4p2 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F4P2] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f4p3 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F4P3] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z])
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f4p4 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F4P4] = LIGHT_MIN + result * LIGHT_RANGE;
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f5p2 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F5P2] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f5p3 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F5P3] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f5p4 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F5P4] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 2) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f5p1 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F5P1] = LIGHT_MIN + result * LIGHT_RANGE;
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f6p2 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F6P2] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f6p3 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F6P3] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f6p4 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F6P4] = LIGHT_MIN + result * LIGHT_RANGE;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z])
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = lightsLevelsValues[average + lightsLevels.f6p1 - blocksLights[(1 * 3 + 1) * 3 + 1]];
				lights[F6P1] = LIGHT_MIN + result * LIGHT_RANGE;
			}
		}
	}

}