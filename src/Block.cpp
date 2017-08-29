#include "Block.h"
#include "Chunk.h"
#include <cstring>

#define BLOCK_SIZE .5
#define LIGHT_MIN .125
#define LIGHT_RANGE .6
#define SSAO_FACTOR 2

namespace voxel
{

	static float lightsLevels[] = {0.03518, 0.04398, 0.05497, 0.06871, 0.08589, 0.10737, 0.13421, 0.16777, 0.20971, .26214, .32768, .4096, .512, .64, .8, 1};

	Block::Block(uint8_t type)
	: type(type)
	{
		//Empty
	}

	void Block::fillBuffers(Chunk *chunk, glm::vec3 &pos, std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices)
	{
		if (this->type == 0)
			return;
		uint8_t visibleFaces;
		calcVisibleFaces(chunk, pos, visibleFaces);
		BlockLights lights;
		calcLights(chunk, pos, lights, visibleFaces);
		glm::vec3 color(1, 1, 1);
		float f1p1 = lightsLevels[lights.f1p1] * LIGHT_RANGE + LIGHT_MIN;
		float f1p2 = lightsLevels[lights.f1p2] * LIGHT_RANGE + LIGHT_MIN;
		float f1p3 = lightsLevels[lights.f1p3] * LIGHT_RANGE + LIGHT_MIN;
		float f1p4 = lightsLevels[lights.f1p4] * LIGHT_RANGE + LIGHT_MIN;
		float f2p1 = lightsLevels[lights.f2p1] * LIGHT_RANGE + LIGHT_MIN;
		float f2p2 = lightsLevels[lights.f2p2] * LIGHT_RANGE + LIGHT_MIN;
		float f2p3 = lightsLevels[lights.f2p3] * LIGHT_RANGE + LIGHT_MIN;
		float f2p4 = lightsLevels[lights.f2p4] * LIGHT_RANGE + LIGHT_MIN;
		float f3p1 = lightsLevels[lights.f3p1] * LIGHT_RANGE + LIGHT_MIN;
		float f3p2 = lightsLevels[lights.f3p2] * LIGHT_RANGE + LIGHT_MIN;
		float f3p3 = lightsLevels[lights.f3p3] * LIGHT_RANGE + LIGHT_MIN;
		float f3p4 = lightsLevels[lights.f3p4] * LIGHT_RANGE + LIGHT_MIN;
		float f4p1 = lightsLevels[lights.f4p1] * LIGHT_RANGE + LIGHT_MIN;
		float f4p2 = lightsLevels[lights.f4p2] * LIGHT_RANGE + LIGHT_MIN;
		float f4p3 = lightsLevels[lights.f4p3] * LIGHT_RANGE + LIGHT_MIN;
		float f4p4 = lightsLevels[lights.f4p4] * LIGHT_RANGE + LIGHT_MIN;
		float f5p1 = lightsLevels[lights.f5p1] * LIGHT_RANGE + LIGHT_MIN;
		float f5p2 = lightsLevels[lights.f5p2] * LIGHT_RANGE + LIGHT_MIN;
		float f5p3 = lightsLevels[lights.f5p3] * LIGHT_RANGE + LIGHT_MIN;
		float f5p4 = lightsLevels[lights.f5p4] * LIGHT_RANGE + LIGHT_MIN;
		float f6p1 = lightsLevels[lights.f6p1] * LIGHT_RANGE + LIGHT_MIN;
		float f6p2 = lightsLevels[lights.f6p2] * LIGHT_RANGE + LIGHT_MIN;
		float f6p3 = lightsLevels[lights.f6p3] * LIGHT_RANGE + LIGHT_MIN;
		float f6p4 = lightsLevels[lights.f6p4] * LIGHT_RANGE + LIGHT_MIN;
		float texX = ((this->type - 1) % 16) / 16.;
		float texY = ((this->type - 1) / 16) / 16.;
		if (this->type == 1)
		{
			texX = 3 / 16.;
			texY = 0 / 16.;
		}
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f1p1 * color.x, f1p1 * color.y, f1p1 * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f1p2 * color.x, f1p2 * color.y, f1p2 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f1p3 * color.x, f1p3 * color.y, f1p3 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f1p4 * color.x, f1p4 * color.y, f1p4 * color.z));
			if (lights.f1p2 + lights.f1p4 > lights.f1p1 + lights.f1p3)
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
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f2p1 * color.x, f2p1 * color.y, f2p1 * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f2p2 * color.x, f2p2 * color.y, f2p2 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f2p3 * color.x, f2p3 * color.y, f2p3 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f2p4 * color.x, f2p4 * color.y, f2p4 * color.z));
			if (lights.f2p2 + lights.f2p4 <= lights.f2p1 + lights.f2p3)
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
			colors.push_back(glm::vec3(f3p1 * color.x, f3p1 * color.y, f3p1 * color.z));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f3p2 * color.x, f3p2 * color.y, f3p2 * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f3p3 * color.x, f3p3 * color.y, f3p3 * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f3p4 * color.x, f3p4 * color.y, f3p4 * color.z));
			if (lights.f3p2 + lights.f3p4 > lights.f3p1 + lights.f3p3)
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
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f4p1 * color.x, f4p1 * color.y, f4p1 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f4p2 * color.x, f4p2 * color.y, f4p2 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f4p3 * color.x, f4p3 * color.y, f4p3 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f4p4 * color.x, f4p4 * color.y, f4p4 * color.z));
			if (lights.f4p2 + lights.f4p4 < lights.f4p1 + lights.f4p3)
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
			float oldTexX = texX;
			float oldTexY = texY;
			if (this->type == 1)
			{
				texX = 0;
				texY = 0;
			}
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f5p1 * color.x, f5p1 * color.y, f5p1 * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f5p2 * color.x, f5p2 * color.y, f5p2 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f5p3 * color.x, f5p3 * color.y, f5p3 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f5p4 * color.x, f5p4 * color.y, f5p4 * color.z));
			if (lights.f5p2 + lights.f5p4 > lights.f5p1 + lights.f5p3)
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
				texX = oldTexX;
				texY = oldTexY;
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f6p1 * color.x, f6p1 * color.y, f6p1 * color.z));
			vertexes.push_back(glm::vec3(pos.x - BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f6p2 * color.x, f6p2 * color.y, f6p2 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f6p3 * color.x, f6p3 * color.y, f6p3 * color.z));
			vertexes.push_back(glm::vec3(pos.x + BLOCK_SIZE, pos.y - BLOCK_SIZE, pos.z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f6p4 * color.x, f6p4 * color.y, f6p4 * color.z));
			if (lights.f6p2 + lights.f6p4 < lights.f6p1 + lights.f6p3)
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

	bool Block::calcLightsIsTransparentChunk(Chunk *chunk, int32_t x, int32_t y, int32_t z)
	{
		if (!chunk)
			return (true);
		Block *tmp = chunk->getBlockAt(x, y, z);
		return (!tmp || tmp->isTransparent());
	}

	bool Block::calcLightsIsTransparent(Chunk *chunk, glm::vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return (1);
		bool different = (addX < 0 && pos.x - chunk->getX() < -addX)
			|| (addX > 0 && pos.x - chunk->getX() + addX >= CHUNK_WIDTH)
			|| (addZ < 0 && pos.z - chunk->getZ() < -addZ)
			|| (addZ > 0 && pos.z - chunk->getZ() + addZ >= CHUNK_WIDTH);
		if (different)
		{
			int32_t newX = pos.x - chunk->getX() + addX;
			int32_t newY = pos.y + addY;
			int32_t newZ = pos.z - chunk->getZ() + addZ;
			Chunk *newChunk = chunk;
			if (newX < 0)
			{
				newX += CHUNK_WIDTH;
				if (!(newChunk = newChunk->getChunkXLess()))
					return (true);
			}
			if (newX >= CHUNK_WIDTH)
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
			if (newZ >= CHUNK_WIDTH)
			{
				newZ -= CHUNK_WIDTH;
				if (!(newChunk = newChunk->getChunkZMore()))
					return (true);
			}
			return (calcLightsIsTransparentChunk(newChunk, newX, newY, newZ));
		}
		return (calcLightsIsTransparentChunk(chunk, pos.x - chunk->getX() + addX, pos.y + addY, pos.z - chunk->getZ() + addZ));
	}

	void Block::calcLights(Chunk *chunk, glm::vec3 &pos, BlockLights &lights, uint8_t visibleFaces)
	{
		std::memset(&lights, 0xff, sizeof(lights));
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			if (!calcLightsIsTransparent(chunk, pos, 1, -1, 1))
				lights.f1p4 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 0, -1, 1))
			{
				lights.f1p4 -= SSAO_FACTOR;
				lights.f1p1 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, -1, -1, 1))
				lights.f1p1 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, -1, 1, 1))
				lights.f1p2 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 0, 1, 1))
			{
				lights.f1p2 -= SSAO_FACTOR;
				lights.f1p3 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, 1, 1, 1))
				lights.f1p3 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 1, 0, 1))
			{
				lights.f1p3 -= SSAO_FACTOR;
				lights.f1p4 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, -1, 0, 1))
			{
				lights.f1p1 -= SSAO_FACTOR;
				lights.f1p2 -= SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			if (!calcLightsIsTransparent(chunk, pos, 1, -1, -1))
				lights.f2p4 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 0, -1, -1))
			{
				lights.f2p4 -= SSAO_FACTOR;
				lights.f2p1 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, -1, -1, -1))
				lights.f2p1 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, -1, 1, -1))
				lights.f2p2 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 0, 1, -1))
			{
				lights.f2p2 -= SSAO_FACTOR;
				lights.f2p3 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, 1, 1, -1))
				lights.f2p3 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 1, 0, -1))
			{
				lights.f2p3 -= SSAO_FACTOR;
				lights.f2p4 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, -1, 0, -1))
			{
				lights.f2p1 -= SSAO_FACTOR;
				lights.f2p2 -= SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			if (!calcLightsIsTransparent(chunk, pos, -1, -1, 1))
				lights.f3p4 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, -1, -1, 0))
			{
				lights.f3p4 -= SSAO_FACTOR;
				lights.f3p1 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, -1, -1, -1))
				lights.f3p1 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, -1, 1, -1))
				lights.f3p2 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, -1, 1, 0))
			{
				lights.f3p2 -= SSAO_FACTOR;
				lights.f3p3 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, -1, 1, 1))
				lights.f3p3 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, -1, 0, 1))
			{
				lights.f3p3 -= SSAO_FACTOR;
				lights.f3p4 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, -1, 0, -1))
			{
				lights.f3p1 -= SSAO_FACTOR;
				lights.f3p2 -= SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			if (!calcLightsIsTransparent(chunk, pos, 1, -1, 1))
				lights.f4p4 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 1, -1, 0))
			{
				lights.f4p4 -= SSAO_FACTOR;
				lights.f4p1 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, 1, -1, -1))
				lights.f4p1 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 1, 1, -1))
				lights.f4p2 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 1, 1, 0))
			{
				lights.f4p2 -= SSAO_FACTOR;
				lights.f4p3 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, 1, 1, 1))
				lights.f4p3 -= SSAO_FACTOR;
			if (!calcLightsIsTransparent(chunk, pos, 1, 0, 1))
			{
				lights.f4p3 -= SSAO_FACTOR;
				lights.f4p4 -= SSAO_FACTOR;
			}
			if (!calcLightsIsTransparent(chunk, pos, 1, 0, -1))
			{
				lights.f4p1 -= SSAO_FACTOR;
				lights.f4p2 -= SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			if (pos.y < CHUNK_HEIGHT - 1)
			{
				if (!calcLightsIsTransparent(chunk, pos, -1, 1, -1))
					lights.f5p2 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, -1, 1, 0))
				{
					lights.f5p1 -= SSAO_FACTOR;
					lights.f5p2 -= SSAO_FACTOR;
				}
				if (!calcLightsIsTransparent(chunk, pos, -1, 1, 1))
					lights.f5p1 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, 1, 1, -1))
					lights.f5p3 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, 1, 1, 0))
				{
					lights.f5p3 -= SSAO_FACTOR;
					lights.f5p4 -= SSAO_FACTOR;
				}
				if (!calcLightsIsTransparent(chunk, pos, 1, 1, 1))
					lights.f5p4 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, 0, 1, -1))
				{
					lights.f5p2 -= SSAO_FACTOR;
					lights.f5p3 -= SSAO_FACTOR;
				}
				if (!calcLightsIsTransparent(chunk, pos, 0, 1, 1))
				{
					lights.f5p4 -= SSAO_FACTOR;
					lights.f5p1 -= SSAO_FACTOR;
				}
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			if (pos.y < 0)
			{
				if (!calcLightsIsTransparent(chunk, pos, -1, -1, 1))
					lights.f6p1 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, -1, -1, 0))
				{
					lights.f6p1 -= SSAO_FACTOR;
					lights.f6p2 -= SSAO_FACTOR;
				}
				if (!calcLightsIsTransparent(chunk, pos, -1, -1, -1))
					lights.f6p2 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, 1, -1, -1))
					lights.f6p3 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, 1, -1, 0))
				{
					lights.f6p3 -= SSAO_FACTOR;
					lights.f6p4 -= SSAO_FACTOR;
				}
				if (!calcLightsIsTransparent(chunk, pos, 1, -1, 1))
					lights.f6p4 -= SSAO_FACTOR;
				if (!calcLightsIsTransparent(chunk, pos, 0, -1, -1))
				{
					lights.f6p2 -= SSAO_FACTOR;
					lights.f6p3 -= SSAO_FACTOR;
				}
				if (!calcLightsIsTransparent(chunk, pos, 0, -1, 1))
				{
					lights.f6p4 -= SSAO_FACTOR;
					lights.f6p1 -= SSAO_FACTOR;
				}
			}
		}
	}

}
