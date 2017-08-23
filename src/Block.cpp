#include "Block.h"
#include "Chunk.h"
#include "Debug.h"
#include <cstring>

#define BLOCK_SIZE .5

namespace voxel
{

	Block::Block(Chunk *chunk, int32_t x, int32_t y, int32_t z, uint8_t type)
	: chunk(chunk)
	, type(type)
	, x(x)
	, y(y)
	, z(z)
	{
		//Empty
	}

	void Block::fillBuffers(std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices)
	{
		if (this->type == 0)
			return;
		calcVisibleFaces();
		BlockLights lights;
		calcLights(lights);
		float red;
		float green;
		float blue;
		if (this->type == 1)
		{
			red = 44 / 255.;
			green = 130 / 255.;
			blue = 201 / 255.;
		}
		else if (this->type == 2)
		{
			red = 247 / 255.;
			green = 218 / 255.;
			blue = 100 / 255.;
		}
		else
		{
			red = 65 / 255.;
			green = 168 / 255.;
			blue = 95 / 255.;
		}
		float f1p1 = lights.f1p1 / 16.;
		float f1p2 = lights.f1p2 / 16.;
		float f1p3 = lights.f1p3 / 16.;
		float f1p4 = lights.f1p4 / 16.;
		float f2p1 = lights.f2p1 / 16.;
		float f2p2 = lights.f2p2 / 16.;
		float f2p3 = lights.f2p3 / 16.;
		float f2p4 = lights.f2p4 / 16.;
		float f3p1 = lights.f3p1 / 16.;
		float f3p2 = lights.f3p2 / 16.;
		float f3p3 = lights.f3p3 / 16.;
		float f3p4 = lights.f3p4 / 16.;
		float f4p1 = lights.f4p1 / 16.;
		float f4p2 = lights.f4p2 / 16.;
		float f4p3 = lights.f4p3 / 16.;
		float f4p4 = lights.f4p4 / 16.;
		float f5p1 = lights.f5p1 / 16.;
		float f5p2 = lights.f5p2 / 16.;
		float f5p3 = lights.f5p3 / 16.;
		float f5p4 = lights.f5p4 / 16.;
		float f6p1 = lights.f6p1 / 16.;
		float f6p2 = lights.f6p2 / 16.;
		float f6p3 = lights.f6p3 / 16.;
		float f6p4 = lights.f6p4 / 16.;
		if (this->visibleFaces & BLOCK_FACE_FRONT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f1p1 * red, f1p1 * green, f1p1 * blue));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f1p2 * red, f1p2 * green, f1p2 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f1p3 * red, f1p3 * green, f1p3 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f1p4 * red, f1p4 * green, f1p4 * blue));
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
		if (this->visibleFaces & BLOCK_FACE_BACK)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f2p1 * red, f2p1 * green, f2p1 * blue));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f2p2 * red, f2p2 * green, f2p2 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f2p3 * red, f2p3 * green, f2p3 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f2p4 * red, f2p4 * green, f2p4 * blue));
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
		if (this->visibleFaces & BLOCK_FACE_LEFT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f3p1 * red, f3p1 * green, f3p1 * blue));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f3p2 * red, f3p2 * green, f3p2 * blue));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f3p3 * red, f3p3 * green, f3p3 * blue));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f3p4 * red, f3p4 * green, f3p4 * blue));
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
		if (this->visibleFaces & BLOCK_FACE_RIGHT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f4p1 * red, f4p1 * green, f4p1 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f4p2 * red, f4p2 * green, f4p2 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f4p3 * red, f4p3 * green, f4p3 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f4p4 * red, f4p4 * green, f4p4 * blue));
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
		if (this->visibleFaces & BLOCK_FACE_UP)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f5p1 * red, f5p1 * green, f5p1 * blue));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f5p2 * red, f5p2 * green, f5p2 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f5p3 * red, f5p3 * green, f5p3 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f5p4 * red, f5p4 * green, f5p4 * blue));
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
		}
		if (this->visibleFaces & BLOCK_FACE_DOWN)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f6p1 * red, f6p1 * green, f6p1 * blue));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f6p2 * red, f6p2 * green, f6p2 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f6p3 * red, f6p3 * green, f6p3 * blue));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f6p4 * red, f6p4 * green, f6p4 * blue));
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

	void Block::calcVisibleFaces()
	{
		this->visibleFaces = 0;
		if (this->z - this->chunk->getZ() == CHUNK_WIDTH - 1)
		{
			if (!this->chunk->getChunkZMore())
			{
				this->visibleFaces |= BLOCK_FACE_FRONT;
			}
			else
			{
				Block *block = this->chunk->getChunkZMore()->getBlockAt(this->x - this->chunk->getX(), this->y, 0);
				if (!block || block->isTransparent())
					this->visibleFaces |= BLOCK_FACE_FRONT;
			}
		}
		else
		{
			Block *block = this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y, this->z - this->chunk->getZ() + 1);
			if (!block || block->isTransparent())
				this->visibleFaces |= BLOCK_FACE_FRONT;
		}
		if (this->z - this->chunk->getZ() == 0)
		{
			if (!this->chunk->getChunkZLess())
			{
				this->visibleFaces |= BLOCK_FACE_BACK;
			}
			else
			{
				Block *block = this->chunk->getChunkZLess()->getBlockAt(this->x - this->chunk->getX(), this->y, CHUNK_WIDTH - 1);
				if (!block || block->isTransparent())
					this->visibleFaces |= BLOCK_FACE_BACK;
			}
		}
		else
		{
			Block *block = this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y, this->z - this->chunk->getZ() - 1);
			if (!block || block->isTransparent())
				this->visibleFaces |= BLOCK_FACE_BACK;
		}
		if (this->x - this->chunk->getX() == 0)
		{
			if (!this->chunk->getChunkXLess())
			{
				this->visibleFaces |= BLOCK_FACE_LEFT;
			}
			else
			{
				Block *block = this->chunk->getChunkXLess()->getBlockAt(CHUNK_WIDTH - 1, this->y, this->z - this->chunk->getZ());
				if (!block || block->isTransparent())
					this->visibleFaces |= BLOCK_FACE_LEFT;
			}
		}
		else
		{
			Block *block = this->chunk->getBlockAt(this->x - this->chunk->getX() - 1, this->y, this->z - this->chunk->getZ());
			if (!block || block->isTransparent())
				this->visibleFaces |= BLOCK_FACE_LEFT;
		}
		if (this->x - this->chunk->getX() == CHUNK_WIDTH - 1)
		{
			if (!this->chunk->getChunkXMore())
			{
				this->visibleFaces |= BLOCK_FACE_RIGHT;
			}
			else
			{
				Block *block = this->chunk->getChunkXMore()->getBlockAt(0, this->y, this->z - this->chunk->getZ());
				if (!block || block->isTransparent())
					this->visibleFaces |= BLOCK_FACE_RIGHT;
			}
		}
		else
		{
			Block *block = this->chunk->getBlockAt(this->x - this->chunk->getX() + 1, this->y, this->z - this->chunk->getZ());
			if (!block || block->isTransparent())
				this->visibleFaces |= BLOCK_FACE_RIGHT;
		}
		if (this->y == CHUNK_HEIGHT - 1)
		{
			this->visibleFaces |= BLOCK_FACE_UP;
		}
		else
		{
			Block *block = this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y + 1, this->z - this->chunk->getZ());
			if (!block || block->isTransparent())
				this->visibleFaces |= BLOCK_FACE_UP;
		}
		if (this->y == 0)
		{
			this->visibleFaces |= BLOCK_FACE_DOWN;
		}
		else
		{
			Block * block = this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y - 1, this->z - this->chunk->getZ());
			if (!block || block->isTransparent())
				this->visibleFaces |= BLOCK_FACE_DOWN;
		}
	}

	bool Block::calcLightsIsTransparentSameChunk(int8_t addX, int8_t addY, int8_t addZ)
	{
		Block *tmp = this->chunk->getBlockAt(this->x - this->chunk->getX() + addX, this->y + addY, this->z - this->chunk->getZ() + addZ);
		return (!tmp || tmp->isTransparent());
	}

	bool Block::calcLightsIsTransparentOtherChunk(Chunk *chunk, int32_t newBlockX, int32_t newBlockY, int32_t newBlockZ)
	{
		if (!chunk)
			return (true);
		Block *tmp = chunk->getBlockAt(newBlockX, newBlockY, newBlockZ);
		return (!tmp || tmp->isTransparent());
	}

	bool Block::calcLightsIsTransparent(int8_t addX, int8_t addY, int8_t addZ)
	{
		if (this->y + addY < 0 || this->y + addY >= CHUNK_HEIGHT)
			return (1);
		bool different = (addX < 0 && this->x - this->chunk->getX() < -addX)
			|| (addX > 0 && this->x - this->chunk->getX() + addX >= CHUNK_WIDTH)
			|| (addZ < 0 && this->z - this->chunk->getZ() < -addZ)
			|| (addZ > 0 && this->z - this->chunk->getZ() + addZ >= CHUNK_WIDTH);
		if (different)
		{
			int32_t newX = this->x - this->chunk->getX() + addX;
			int32_t newY = this->y + addY;
			int32_t newZ = this->z - this->chunk->getZ() + addZ;
			Chunk *newChunk = this->chunk;
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
			return (calcLightsIsTransparentOtherChunk(newChunk, newX, newY, newZ));
		}
		return (calcLightsIsTransparentSameChunk(addX, addY, addZ));
	}

	void Block::calcLights(BlockLights &lights)
	{
		std::memset(&lights, 0xff, sizeof(lights));
#define LESS 1
		if (this->visibleFaces & BLOCK_FACE_FRONT)
		{
			if (!calcLightsIsTransparent(1, -1, 1))
				lights.f1p4 -= LESS;
			if (!calcLightsIsTransparent(0, -1, 1))
			{
				lights.f1p4 -= LESS;
				lights.f1p1 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, -1, 1))
				lights.f1p1 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, 1))
				lights.f1p2 -= LESS;
			if (!calcLightsIsTransparent(0, 1, 1))
			{
				lights.f1p2 -= LESS;
				lights.f1p3 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 1, 1))
				lights.f1p3 -= LESS;
			if (!calcLightsIsTransparent(1, 0, 1))
			{
				lights.f1p3 -= LESS;
				lights.f1p4 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 0, 1))
			{
				lights.f1p1 -= LESS;
				lights.f1p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_BACK)
		{
			if (!calcLightsIsTransparent(1, -1, -1))
				lights.f2p4 -= LESS;
			if (!calcLightsIsTransparent(0, -1, -1))
			{
				lights.f2p4 -= LESS;
				lights.f2p1 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, -1, -1))
				lights.f2p1 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, -1))
				lights.f2p2 -= LESS;
			if (!calcLightsIsTransparent(0, 1, -1))
			{
				lights.f2p2 -= LESS;
				lights.f2p3 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 1, -1))
				lights.f2p3 -= LESS;
			if (!calcLightsIsTransparent(1, 0, -1))
			{
				lights.f2p3 -= LESS;
				lights.f2p4 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 0, -1))
			{
				lights.f2p1 -= LESS;
				lights.f2p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_LEFT)
		{
			if (!calcLightsIsTransparent(-1, -1, 1))
				lights.f3p4 -= LESS;
			if (!calcLightsIsTransparent(-1, -1, 0))
			{
				lights.f3p4 -= LESS;
				lights.f3p1 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, -1, -1))
				lights.f3p1 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, -1))
				lights.f3p2 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, 0))
			{
				lights.f3p2 -= LESS;
				lights.f3p3 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 1, 1))
				lights.f3p3 -= LESS;
			if (!calcLightsIsTransparent(-1, 0, 1))
			{
				lights.f3p3 -= LESS;
				lights.f3p4 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 0, -1))
			{
				lights.f3p1 -= LESS;
				lights.f3p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_RIGHT)
		{
			if (!calcLightsIsTransparent(1, -1, 1))
				lights.f4p4 -= LESS;
			if (!calcLightsIsTransparent(1, -1, 0))
			{
				lights.f4p4 -= LESS;
				lights.f4p1 -= LESS;
			}
			if (!calcLightsIsTransparent(1, -1, -1))
				lights.f4p1 -= LESS;
			if (!calcLightsIsTransparent(1, 1, -1))
				lights.f4p2 -= LESS;
			if (!calcLightsIsTransparent(1, 1, 0))
			{
				lights.f4p2 -= LESS;
				lights.f4p3 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 1, 1))
				lights.f4p3 -= LESS;
			if (!calcLightsIsTransparent(1, 0, 1))
			{
				lights.f4p3 -= LESS;
				lights.f4p4 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 0, -1))
			{
				lights.f4p1 -= LESS;
				lights.f4p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_UP)
		{
			if (this->y < CHUNK_HEIGHT - 1)
			{
				if (!calcLightsIsTransparent(-1, 1, -1))
					lights.f5p2 -= LESS;
				if (!calcLightsIsTransparent(-1, 1, 0))
				{
					lights.f5p1 -= LESS;
					lights.f5p2 -= LESS;
				}
				if (!calcLightsIsTransparent(-1, 1, 1))
					lights.f5p1 -= LESS;
				if (!calcLightsIsTransparent(1, 1, -1))
					lights.f5p3 -= LESS;
				if (!calcLightsIsTransparent(1, 1, 0))
				{
					lights.f5p3 -= LESS;
					lights.f5p4 -= LESS;
				}
				if (!calcLightsIsTransparent(1, 1, 1))
					lights.f5p4 -= LESS;
				if (!calcLightsIsTransparent(0, 1, -1))
				{
					lights.f5p2 -= LESS;
					lights.f5p3 -= LESS;
				}
				if (!calcLightsIsTransparent(0, 1, 1))
				{
					lights.f5p4 -= LESS;
					lights.f5p1 -= LESS;
				}
			}
		}
		if (this->visibleFaces & BLOCK_FACE_DOWN)
		{
			if (this->y < 0)
			{
				if (!calcLightsIsTransparent(-1, -1, 1))
					lights.f6p1 -= LESS;
				if (!calcLightsIsTransparent(-1, -1, 0))
				{
					lights.f6p1 -= LESS;
					lights.f6p2 -= LESS;
				}
				if (!calcLightsIsTransparent(-1, -1, -1))
					lights.f6p2 -= LESS;
				if (!calcLightsIsTransparent(1, -1, -1))
					lights.f6p3 -= LESS;
				if (!calcLightsIsTransparent(1, -1, 0))
				{
					lights.f6p3 -= LESS;
					lights.f6p4 -= LESS;
				}
				if (!calcLightsIsTransparent(1, -1, 1))
					lights.f6p4 -= LESS;
				if (!calcLightsIsTransparent(0, -1, -1))
				{
					lights.f6p2 -= LESS;
					lights.f6p3 -= LESS;
				}
				if (!calcLightsIsTransparent(0, -1, 1))
				{
					lights.f6p4 -= LESS;
					lights.f6p1 -= LESS;
				}
			}
		}
	}

}
