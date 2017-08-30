#include "Block.h"
#include "Chunk.h"
#include <cstring>
#include "Debug.h"

#define BLOCK_SIZE .5
#define LIGHT_MIN .125
#define LIGHT_RANGE .6
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

#define LIGHT_SSAO_TEST(light, ssao, base) if (base <= ssao) {light = 0;} else {light = base - ssao;}
#define SMOOTH_TEST_LESS(lights, changed, index, diff) if (!changed[index]) {lights[index] -= diff;changed[index] = true;}
#define SMOOTH_TEST_MORE(lights, changed, index, diff) if (!changed[index]) {lights[index] += diff;changed[index] = true;}

namespace voxel
{

	static float lightsLevelsValues[] = {0.03518, 0.04398, 0.05497, 0.06871, 0.08589, 0.10737, 0.13421, 0.16777, 0.20971, .26214, .32768, .4096, .512, .64, .8, 1};

	Block::Block(uint8_t type)
	: type(type)
	{
		//Empty
	}

	Block::Block()
	: type(0)
	{
		//Empty
	}

	void Block::fillBuffers(Chunk *chunk, glm::vec3 &pos, std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices)
	{
		if (this->type == 0)
			return;
		uint8_t visibleFaces;
		calcVisibleFaces(chunk, pos, visibleFaces);
		if (!visibleFaces)
			return;
		BlockLightsLevels lightsLevels;
		calcLightsLevels(chunk, pos, lightsLevels, visibleFaces);
		glm::vec3 color(1, 1, 1);
		float lights[24];
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
		smoothLights(chunk, pos, lights, visibleFaces);
		float texXOrg = ((this->type - 1) % 16) / 16.;
		float texYOrg = ((this->type - 1) / 16) / 16.;
		float texXDst = texXOrg + 1. / 16;
		float texYDst = texYOrg + 1. / 16;
		if (this->type == 1)
		{
			texXOrg = 3 / 16.;
			texYOrg = 0 / 16.;
			texXDst = texXOrg + 1. / 16;
			texYDst = texYOrg + 1. / 16;
		}
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYDst));
			colors.push_back(glm::vec3(lights[F1P1] * color.x, lights[F1P1] * color.y, lights[F1P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYOrg));
			colors.push_back(glm::vec3(lights[F1P2] * color.x, lights[F1P2] * color.y, lights[F1P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYOrg));
			colors.push_back(glm::vec3(lights[F1P3] * color.x, lights[F1P3] * color.y, lights[F1P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYDst));
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
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYDst));
			colors.push_back(glm::vec3(lights[F2P1] * color.x, lights[F2P1] * color.y, lights[F2P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYOrg));
			colors.push_back(glm::vec3(lights[F2P2] * color.x, lights[F2P2] * color.y, lights[F2P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYOrg));
			colors.push_back(glm::vec3(lights[F2P3] * color.x, lights[F2P3] * color.y, lights[F2P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYDst));
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
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYDst));
			colors.push_back(glm::vec3(lights[F3P1] * color.x, lights[F3P1] * color.y, lights[F3P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYOrg));
			colors.push_back(glm::vec3(lights[F3P2] * color.x, lights[F3P2] * color.y, lights[F3P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYOrg));
			colors.push_back(glm::vec3(lights[F3P3] * color.x, lights[F3P3] * color.y, lights[F3P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYDst));
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
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYDst));
			colors.push_back(glm::vec3(lights[F4P1] * color.x, lights[F4P1] * color.y, lights[F4P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYOrg));
			colors.push_back(glm::vec3(lights[F4P2] * color.x, lights[F4P2] * color.y, lights[F4P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYOrg));
			colors.push_back(glm::vec3(lights[F4P3] * color.x, lights[F4P3] * color.y, lights[F4P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYDst));
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
			float oldTexXOrg = texXOrg;
			float oldTexYOrg = texYOrg;
			float oldTexXDst = texXDst;
			float oldTexYDst = texYDst;
			if (this->type == 1)
			{
				texXOrg = 0;
				texYOrg = 0;
				texXDst = texXOrg + 1. / 16;
				texYDst = texYOrg + 1. / 16;
			}
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYOrg));
			colors.push_back(glm::vec3(lights[F5P1] * color.x, lights[F5P1] * color.y, lights[F5P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYDst));
			colors.push_back(glm::vec3(lights[F5P2] * color.x, lights[F5P2] * color.y, lights[F5P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYDst));
			colors.push_back(glm::vec3(lights[F5P3] * color.x, lights[F5P3] * color.y, lights[F5P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYOrg));
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
			if (this->type == 1)
			{
				texXOrg = oldTexXOrg;
				texYOrg = oldTexYOrg;
				texXDst = oldTexXDst;
				texYDst = oldTexYDst;
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			float oldTexXOrg = texXOrg;
			float oldTexYOrg = texYOrg;
			float oldTexXDst = texXDst;
			float oldTexYDst = texYDst;
			if (this->type == 1)
			{
				texXOrg = 2 / 16.;
				texYOrg = 0;
				texXDst = texXOrg + 1. / 16;
				texYDst = texYOrg + 1. / 16;
			}
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYOrg));
			colors.push_back(glm::vec3(lights[F6P1] * color.x, lights[F6P1] * color.y, lights[F6P1] * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXOrg, texYDst));
			colors.push_back(glm::vec3(lights[F6P2] * color.x, lights[F6P2] * color.y, lights[F6P2] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYDst));
			colors.push_back(glm::vec3(lights[F6P3] * color.x, lights[F6P3] * color.y, lights[F6P3] * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texXDst, texYOrg));
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
			if (this->type == 1)
			{
				texXOrg = oldTexXOrg;
				texYOrg = oldTexYOrg;
				texXDst = oldTexXDst;
				texYDst = oldTexYDst;
			}
		}
	}

	void Block::calcVisibleFaces(Chunk *chunk, glm::vec3 &pos, uint8_t &visibleFaces)
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
				Block *block = chunk->getChunkZMore()->getBlockAt(pos.x - chunk->getX(), pos.y, 0);
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_FRONT;
			}
		}
		else
		{
			Block *block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() + 1);
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
				Block *block = chunk->getChunkZLess()->getBlockAt(pos.x - chunk->getX(), pos.y, CHUNK_WIDTH - 1);
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_BACK;
			}
		}
		else
		{
			Block *block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() - 1);
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
				Block *block = chunk->getChunkXLess()->getBlockAt(CHUNK_WIDTH - 1, pos.y, pos.z - chunk->getZ());
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_LEFT;
			}
		}
		else
		{
			Block *block = chunk->getBlockAt(pos.x - chunk->getX() - 1, pos.y, pos.z - chunk->getZ());
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
				Block *block = chunk->getChunkXMore()->getBlockAt(0, pos.y, pos.z - chunk->getZ());
				if (!block || (block->isTransparent() && block->getType() != this->type))
					visibleFaces |= BLOCK_FACE_RIGHT;
			}
		}
		else
		{
			Block *block = chunk->getBlockAt(pos.x - chunk->getX() + 1, pos.y, pos.z - chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_RIGHT;
		}
		if (pos.y == CHUNK_HEIGHT - 1)
		{
			visibleFaces |= BLOCK_FACE_UP;
		}
		else
		{
			Block *block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y + 1, pos.z - chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_UP;
		}
		if (pos.y == 0)
		{
			visibleFaces |= BLOCK_FACE_DOWN;
		}
		else
		{
			Block * block = chunk->getBlockAt(pos.x - chunk->getX(), pos.y - 1, pos.z - chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				visibleFaces |= BLOCK_FACE_DOWN;
		}
	}

	bool Block::calcLightsLevelsIsTransparentChunk(Chunk *chunk, int32_t x, int32_t y, int32_t z)
	{
		if (!chunk)
			return (true);
		Block *tmp = chunk->getBlockAt(x, y, z);
		return (!tmp || tmp->isTransparent());
	}

	bool Block::calcLightsLevelsIsTransparent(Chunk *chunk, glm::vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return (1);
		int32_t newX = pos.x - chunk->getX() + addX;
		int32_t newY = pos.y + addY;
		int32_t newZ = pos.z - chunk->getZ() + addZ;
		bool different = (addX < 0 && newX < 0)
			|| (addX > 0 && newX >= CHUNK_WIDTH)
			|| (addZ < 0 && newZ < 0)
			|| (addZ > 0 && newZ >= CHUNK_WIDTH);
		if (different)
		{
			Chunk *newChunk = chunk;
			if (newX < 0)
			{
				newX += CHUNK_WIDTH;
				if (!(newChunk = newChunk->getChunkXLess()))
					return (true);
			}
			else if (newX >= CHUNK_WIDTH)
			{
				newX -= CHUNK_WIDTH;
				if (!(newChunk = newChunk->getChunkXMore()))
					return (true);
			}
			if (newZ < 0)
			{
				newZ += CHUNK_WIDTH;
				if (!(newChunk = newChunk->getChunkZLess()))
					return (true);
			}
			else if (newZ >= CHUNK_WIDTH)
			{
				newZ -= CHUNK_WIDTH;
				if (!(newChunk = newChunk->getChunkZMore()))
					return (true);
			}
			return (calcLightsLevelsIsTransparentChunk(newChunk, newX, newY, newZ));
		}
		return (calcLightsLevelsIsTransparentChunk(chunk, newX, newY, newZ));
	}

	void Block::calcLightsLevels(Chunk *chunk, glm::vec3 &pos, BlockLightsLevels &lights, uint8_t visibleFaces)
	{
		BlockLightsLevels ssao;
		std::memset(&ssao, 0x00, sizeof(ssao));
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, 1))
				ssao.f1p4 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 0, -1, 1))
			{
				ssao.f1p4 += SSAO_FACTOR;
				ssao.f1p1 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, 1))
				ssao.f1p1 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, 1))
				ssao.f1p2 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 0, 1, 1))
			{
				ssao.f1p2 += SSAO_FACTOR;
				ssao.f1p3 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, 1))
				ssao.f1p3 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 0, 1))
			{
				ssao.f1p3 += SSAO_FACTOR;
				ssao.f1p4 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 0, 1))
			{
				ssao.f1p1 += SSAO_FACTOR;
				ssao.f1p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, -1))
				ssao.f2p4 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 0, -1, -1))
			{
				ssao.f2p4 += SSAO_FACTOR;
				ssao.f2p1 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, -1))
				ssao.f2p1 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, -1))
				ssao.f2p2 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 0, 1, -1))
			{
				ssao.f2p2 += SSAO_FACTOR;
				ssao.f2p3 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, -1))
				ssao.f2p3 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 0, -1))
			{
				ssao.f2p3 += SSAO_FACTOR;
				ssao.f2p4 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 0, -1))
			{
				ssao.f2p1 += SSAO_FACTOR;
				ssao.f2p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, 1))
				ssao.f3p4 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, 0))
			{
				ssao.f3p4 += SSAO_FACTOR;
				ssao.f3p1 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, -1))
				ssao.f3p1 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, -1))
				ssao.f3p2 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, 0))
			{
				ssao.f3p2 += SSAO_FACTOR;
				ssao.f3p3 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, 1))
				ssao.f3p3 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 0, 1))
			{
				ssao.f3p3 += SSAO_FACTOR;
				ssao.f3p4 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 0, -1))
			{
				ssao.f3p1 += SSAO_FACTOR;
				ssao.f3p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, 1))
				ssao.f4p4 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, 0))
			{
				ssao.f4p4 += SSAO_FACTOR;
				ssao.f4p1 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, -1))
				ssao.f4p1 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, -1))
				ssao.f4p2 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, 0))
			{
				ssao.f4p2 += SSAO_FACTOR;
				ssao.f4p3 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, 1))
				ssao.f4p3 += SSAO_FACTOR;
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 0, 1))
			{
				ssao.f4p3 += SSAO_FACTOR;
				ssao.f4p4 += SSAO_FACTOR;
			}
			if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 0, -1))
			{
				ssao.f4p1 += SSAO_FACTOR;
				ssao.f4p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			if (pos.y < CHUNK_HEIGHT - 1)
			{
				if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, -1))
					ssao.f5p2 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, 0))
				{
					ssao.f5p1 += SSAO_FACTOR;
					ssao.f5p2 += SSAO_FACTOR;
				}
				if (!calcLightsLevelsIsTransparent(chunk, pos, -1, 1, 1))
					ssao.f5p1 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, -1))
					ssao.f5p3 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, 0))
				{
					ssao.f5p3 += SSAO_FACTOR;
					ssao.f5p4 += SSAO_FACTOR;
				}
				if (!calcLightsLevelsIsTransparent(chunk, pos, 1, 1, 1))
					ssao.f5p4 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, 0, 1, -1))
				{
					ssao.f5p2 += SSAO_FACTOR;
					ssao.f5p3 += SSAO_FACTOR;
				}
				if (!calcLightsLevelsIsTransparent(chunk, pos, 0, 1, 1))
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
				if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, 1))
					ssao.f6p1 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, 0))
				{
					ssao.f6p1 += SSAO_FACTOR;
					ssao.f6p2 += SSAO_FACTOR;
				}
				if (!calcLightsLevelsIsTransparent(chunk, pos, -1, -1, -1))
					ssao.f6p2 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, -1))
					ssao.f6p3 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, 0))
				{
					ssao.f6p3 += SSAO_FACTOR;
					ssao.f6p4 += SSAO_FACTOR;
				}
				if (!calcLightsLevelsIsTransparent(chunk, pos, 1, -1, 1))
					ssao.f6p4 += SSAO_FACTOR;
				if (!calcLightsLevelsIsTransparent(chunk, pos, 0, -1, -1))
				{
					ssao.f6p2 += SSAO_FACTOR;
					ssao.f6p3 += SSAO_FACTOR;
				}
				if (!calcLightsLevelsIsTransparent(chunk, pos, 0, -1, 1))
				{
					ssao.f6p4 += SSAO_FACTOR;
					ssao.f6p1 += SSAO_FACTOR;
				}
			}
		}
		uint8_t blockLight = chunk->getLightAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ()) & 0xf;
		LIGHT_SSAO_TEST(lights.f1p1, ssao.f1p1, blockLight);
		LIGHT_SSAO_TEST(lights.f1p2, ssao.f1p2, blockLight);
		LIGHT_SSAO_TEST(lights.f1p3, ssao.f1p3, blockLight);
		LIGHT_SSAO_TEST(lights.f1p4, ssao.f1p4, blockLight);
		LIGHT_SSAO_TEST(lights.f2p1, ssao.f2p1, blockLight);
		LIGHT_SSAO_TEST(lights.f2p2, ssao.f2p2, blockLight);
		LIGHT_SSAO_TEST(lights.f2p3, ssao.f2p3, blockLight);
		LIGHT_SSAO_TEST(lights.f2p4, ssao.f2p4, blockLight);
		LIGHT_SSAO_TEST(lights.f3p1, ssao.f3p1, blockLight);
		LIGHT_SSAO_TEST(lights.f3p2, ssao.f3p2, blockLight);
		LIGHT_SSAO_TEST(lights.f3p3, ssao.f3p3, blockLight);
		LIGHT_SSAO_TEST(lights.f3p4, ssao.f3p4, blockLight);
		LIGHT_SSAO_TEST(lights.f4p1, ssao.f4p1, blockLight);
		LIGHT_SSAO_TEST(lights.f4p2, ssao.f4p2, blockLight);
		LIGHT_SSAO_TEST(lights.f4p3, ssao.f4p3, blockLight);
		LIGHT_SSAO_TEST(lights.f4p4, ssao.f4p4, blockLight);
		LIGHT_SSAO_TEST(lights.f5p1, ssao.f5p1, blockLight);
		LIGHT_SSAO_TEST(lights.f5p2, ssao.f5p2, blockLight);
		LIGHT_SSAO_TEST(lights.f5p3, ssao.f5p3, blockLight);
		LIGHT_SSAO_TEST(lights.f5p4, ssao.f5p4, blockLight);
		LIGHT_SSAO_TEST(lights.f6p1, ssao.f6p1, blockLight);
		LIGHT_SSAO_TEST(lights.f6p2, ssao.f6p2, blockLight);
		LIGHT_SSAO_TEST(lights.f6p3, ssao.f6p3, blockLight);
		LIGHT_SSAO_TEST(lights.f6p4, ssao.f6p4, blockLight);
	}

	void Block::smoothLights(Chunk *chunk, glm::vec3 &pos, float *lights, uint8_t visibleFaces)
	{
		bool changedLess[24];
		bool changedMore[24];
		std::memset(changedLess, 0, sizeof(changedLess));
		std::memset(changedMore, 0, sizeof(changedMore));
		uint8_t lightLevel = chunk->getLightAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ());
		float diff = LIGHT_RANGE / 16 / 2;
		if (!(visibleFaces & BLOCK_FACE_LEFT) || !calcLightsLevelsIsTransparent(chunk, pos, -1, 0, 0))
		{
			uint8_t nearLight;
			if (pos.x - chunk->getX() == 0)
			{
				if (chunk->getChunkXLess())
					nearLight = chunk->getChunkXLess()->getLightAt(CHUNK_WIDTH - 1, pos.y, pos.z - chunk->getZ());
				else
					nearLight = lightLevel;
			}
			else
			{
				nearLight = chunk->getLightAt(pos.x - chunk->getX() - 1, pos.y, pos.z - chunk->getZ());
			}
			if (lightLevel > nearLight)
			{
				SMOOTH_TEST_LESS(lights, changedLess, F1P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F1P2, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F2P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F2P2, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P2, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P2, diff);
			}
			else if (lightLevel < nearLight)
			{
				SMOOTH_TEST_MORE(lights, changedMore, F1P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F1P2, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F2P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F2P2, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P2, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P2, diff);
			}
		}
		if (!(visibleFaces & BLOCK_FACE_RIGHT) || !calcLightsLevelsIsTransparent(chunk, pos, 1, 0, 0))
		{
			uint8_t nearLight;
			if (pos.x - chunk->getX() == CHUNK_WIDTH - 1)
			{
				if (chunk->getChunkXMore())
					nearLight = chunk->getChunkXMore()->getLightAt(0, pos.y, pos.z - chunk->getZ());
				else
					nearLight = lightLevel;
			}
			else
			{
				nearLight = chunk->getLightAt(pos.x - chunk->getX() + 1, pos.y, pos.z - chunk->getZ());
			}
			if (lightLevel > nearLight)
			{
				SMOOTH_TEST_LESS(lights, changedLess, F1P3, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F1P4, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F2P3, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F2P4, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P3, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P4, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P3, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P4, diff);
			}
			else if (lightLevel < nearLight)
			{
				SMOOTH_TEST_MORE(lights, changedMore, F1P3, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F1P4, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F2P3, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F2P4, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P3, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P4, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P3, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P4, diff);
			}
		}
		if (!(visibleFaces & BLOCK_FACE_BACK) || !calcLightsLevelsIsTransparent(chunk, pos, 0, 0, -1))
		{
			uint8_t nearLight;
			if (pos.z - chunk->getZ() == 0)
			{
				if (chunk->getChunkZLess())
					nearLight = chunk->getChunkZLess()->getLightAt(pos.x - chunk->getX(), pos.y, CHUNK_WIDTH - 1);
				else
					nearLight = lightLevel;
			}
			else
			{
				nearLight = chunk->getLightAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() - 1);
			}
			if (lightLevel > nearLight)
			{
				SMOOTH_TEST_LESS(lights, changedLess, F3P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F3P2, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F4P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F4P2, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P2, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P3, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P2, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P3, diff);
			}
			else if (lightLevel < nearLight)
			{
				SMOOTH_TEST_MORE(lights, changedMore, F3P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F3P2, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F4P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F4P2, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P2, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P3, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P2, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P3, diff);
			}
		}
		if (!(visibleFaces & BLOCK_FACE_FRONT) || !calcLightsLevelsIsTransparent(chunk, pos, 0, 0, 1))
		{
			uint8_t nearLight;
			if (pos.z - chunk->getZ() == CHUNK_WIDTH - 1)
			{
				if (chunk->getChunkZMore())
					nearLight = chunk->getChunkZMore()->getLightAt(pos.x - chunk->getX(), pos.y, 0);
				else
					nearLight = lightLevel;
			}
			else
			{
				nearLight = chunk->getLightAt(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() + 1);
			}
			if (lightLevel > nearLight)
			{
				SMOOTH_TEST_LESS(lights, changedLess, F3P3, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F3P4, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F4P3, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F4P4, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F5P4, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P1, diff);
				SMOOTH_TEST_LESS(lights, changedLess, F6P4, diff);
			}
			else if (lightLevel < nearLight)
			{
				SMOOTH_TEST_MORE(lights, changedMore, F3P3, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F3P4, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F4P3, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F4P4, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F5P4, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P1, diff);
				SMOOTH_TEST_MORE(lights, changedMore, F6P4, diff);
			}
		}
		if (pos.y > 0)
		{
			if (!(visibleFaces & BLOCK_FACE_DOWN) || !calcLightsLevelsIsTransparent(chunk, pos, 0, -1, 0))
			{
				uint8_t nearLight = chunk->getLightAt(pos.x - chunk->getX(), pos.y - 1, pos.z - chunk->getZ());
				if (lightLevel > nearLight)
				{
					SMOOTH_TEST_LESS(lights, changedLess, F1P1, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F1P4, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F2P1, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F2P4, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F3P1, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F3P4, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F4P1, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F4P4, diff);
				}
				else if (lightLevel < nearLight)
				{
					SMOOTH_TEST_MORE(lights, changedMore, F1P1, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F1P4, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F2P1, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F2P4, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F3P1, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F3P4, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F4P1, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F4P4, diff);
				}
			}
		}
		if (pos.y < CHUNK_WIDTH - 1)
		{
			if (!(visibleFaces & BLOCK_FACE_UP) || !calcLightsLevelsIsTransparent(chunk, pos, 0, 1, 0))
			{
				uint8_t nearLight = chunk->getLightAt(pos.x - chunk->getX(), pos.y + 1, pos.z - chunk->getZ());
				if (lightLevel > nearLight)
				{
					SMOOTH_TEST_LESS(lights, changedLess, F1P2, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F1P3, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F2P2, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F2P3, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F3P2, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F3P3, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F4P2, diff);
					SMOOTH_TEST_LESS(lights, changedLess, F4P3, diff);
				}
				else if (lightLevel < nearLight)
				{
					SMOOTH_TEST_MORE(lights, changedMore, F1P2, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F1P3, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F2P2, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F2P3, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F3P2, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F3P3, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F4P2, diff);
					SMOOTH_TEST_MORE(lights, changedMore, F4P3, diff);
				}
			}
		}

	}

}
