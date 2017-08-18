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
		calcLights();
		float f1p1 = this->lights.f1p1 / 16.;
		float f1p2 = this->lights.f1p2 / 16.;
		float f1p3 = this->lights.f1p3 / 16.;
		float f1p4 = this->lights.f1p4 / 16.;
		float f2p1 = this->lights.f2p1 / 16.;
		float f2p2 = this->lights.f2p2 / 16.;
		float f2p3 = this->lights.f2p3 / 16.;
		float f2p4 = this->lights.f2p4 / 16.;
		float f3p1 = this->lights.f3p1 / 16.;
		float f3p2 = this->lights.f3p2 / 16.;
		float f3p3 = this->lights.f3p3 / 16.;
		float f3p4 = this->lights.f3p4 / 16.;
		float f4p1 = this->lights.f4p1 / 16.;
		float f4p2 = this->lights.f4p2 / 16.;
		float f4p3 = this->lights.f4p3 / 16.;
		float f4p4 = this->lights.f4p4 / 16.;
		float f5p1 = this->lights.f5p1 / 16.;
		float f5p2 = this->lights.f5p2 / 16.;
		float f5p3 = this->lights.f5p3 / 16.;
		float f5p4 = this->lights.f5p4 / 16.;
		float f6p1 = this->lights.f6p1 / 16.;
		float f6p2 = this->lights.f6p2 / 16.;
		float f6p3 = this->lights.f6p3 / 16.;
		float f6p4 = this->lights.f6p4 / 16.;
		if (this->visibleFaces & BLOCK_FACE_FRONT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f1p1, f1p1, f1p1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f1p2, f1p2, f1p2));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f1p3, f1p3, f1p3));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f1p4, f1p4, f1p4));
			if (this->lights.f1p2 + this->lights.f1p4 > this->lights.f1p1 + this->lights.f1p3)
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
			colors.push_back(glm::vec3(f2p1, f2p1, f2p1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f2p2, f2p2, f2p2));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f2p3, f2p3, f2p3));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f2p4, f2p4, f2p4));
			if (this->lights.f2p2 + this->lights.f2p4 <= this->lights.f2p1 + this->lights.f2p3)
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
			colors.push_back(glm::vec3(f3p1, f3p1, f3p1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f3p2, f3p2, f3p2));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f3p3, f3p3, f3p3));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(f3p4, f3p4, f3p4));
			if (this->lights.f3p2 + this->lights.f3p4 > this->lights.f3p1 + this->lights.f3p3)
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
			colors.push_back(glm::vec3(f4p1, f4p1, f4p1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f4p2, f4p2, f4p2));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f4p3, f4p3, f4p3));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f4p4, f4p4, f4p4));
			if (this->lights.f4p2 + this->lights.f4p4 <= this->lights.f4p1 + this->lights.f4p3)
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
			colors.push_back(glm::vec3(f5p1, f5p1, f5p1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f5p2, f5p2, f5p2));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f5p3, f5p3, f5p3));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f5p4, f5p4, f5p4));
			if (this->lights.f5p2 + this->lights.f5p4 > this->lights.f5p1 + this->lights.f5p3)
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
			colors.push_back(glm::vec3(f6p1, f6p1, f6p1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(f6p2, f6p2, f6p2));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(f6p3, f6p3, f6p3));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(f6p4, f6p4, f6p4));
			if (this->lights.f6p2 + this->lights.f6p4 > this->lights.f6p1 + this->lights.f6p3)
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
				this->visibleFaces |= BLOCK_FACE_FRONT;
			else if (this->chunk->getChunkZMore()->getBlockAt(this->x - this->chunk->getX(), this->y, 0)->isTransparent())
				this->visibleFaces |= BLOCK_FACE_FRONT;
		}
		else if (this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y, this->z - this->chunk->getZ() + 1)->isTransparent())
		{
			this->visibleFaces |= BLOCK_FACE_FRONT;
		}
		if (this->z - this->chunk->getZ() == 0)
		{
			if (!this->chunk->getChunkZLess())
				this->visibleFaces |= BLOCK_FACE_BACK;
			else if (this->chunk->getChunkZLess()->getBlockAt(this->x - this->chunk->getX(), this->y, CHUNK_WIDTH - 1)->isTransparent())
				this->visibleFaces |= BLOCK_FACE_BACK;
		}
		else if (this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y, this->z - this->chunk->getZ() - 1)->isTransparent())
		{
			this->visibleFaces |= BLOCK_FACE_BACK;
		}
		if (this->x - this->chunk->getX() == 0)
		{
			if (!this->chunk->getChunkXLess())
				this->visibleFaces |= BLOCK_FACE_LEFT;
			else if (this->chunk->getChunkXLess()->getBlockAt(CHUNK_WIDTH - 1, this->y, this->z - this->chunk->getZ())->isTransparent())
				this->visibleFaces |= BLOCK_FACE_LEFT;
		}
		else if (this->chunk->getBlockAt(this->x - this->chunk->getX() - 1, this->y, this->z - this->chunk->getZ())->isTransparent())
		{
			this->visibleFaces |= BLOCK_FACE_LEFT;
		}
		if (this->x - this->chunk->getX() == CHUNK_WIDTH - 1)
		{
			if (!this->chunk->getChunkXMore())
				this->visibleFaces |= BLOCK_FACE_RIGHT;
			else if (this->chunk->getChunkXMore()->getBlockAt(0, this->y, this->z - this->chunk->getZ())->isTransparent())
				this->visibleFaces |= BLOCK_FACE_RIGHT;
		}
		else if (this->chunk->getBlockAt(this->x - this->chunk->getX() + 1, this->y, this->z - this->chunk->getZ())->isTransparent())
			this->visibleFaces |= BLOCK_FACE_RIGHT;
		if (this->y == CHUNK_HEIGHT - 1)
			this->visibleFaces |= BLOCK_FACE_UP;
		else if (this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y + 1, this->z - this->chunk->getZ())->isTransparent())
			this->visibleFaces |= BLOCK_FACE_UP;
		if (this->y == 0)
			this->visibleFaces |= BLOCK_FACE_DOWN;
		else if (this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y - 1, this->z - this->chunk->getZ())->isTransparent())
			this->visibleFaces |= BLOCK_FACE_DOWN;
	}

	bool Block::calcLightsIsTransparentSameChunk(int8_t addX, int8_t addY, int8_t addZ)
	{
		Block *tmp = chunk->getBlockAt(this->x - this->chunk->getX() + addX, this->y + addY, this->z - this->chunk->getZ() + addZ);
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

	void Block::calcLights()
	{
		std::memset(&this->lights, 0xff, sizeof(this->lights));
		#define LESS 4
		if (this->visibleFaces & BLOCK_FACE_FRONT)
		{
			if (!calcLightsIsTransparent(1, -1, 1))
				this->lights.f1p4 -= LESS;
			if (!calcLightsIsTransparent(0, -1, 1))
			{
				this->lights.f1p4 -= LESS;
				this->lights.f1p1 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, -1, 1))
				this->lights.f1p1 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, 1))
				this->lights.f1p2 -= LESS;
			if (!calcLightsIsTransparent(0, 1, 1))
			{
				this->lights.f1p2 -= LESS;
				this->lights.f1p3 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 1, 1))
				this->lights.f1p3 -= LESS;
			if (!calcLightsIsTransparent(1, 0, 1))
			{
				this->lights.f1p3 -= LESS;
				this->lights.f1p4 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 0, 1))
			{
				this->lights.f1p1 -= LESS;
				this->lights.f1p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_BACK)
		{
			if (!calcLightsIsTransparent(1, -1, -1))
				this->lights.f2p4 -= LESS;
			if (!calcLightsIsTransparent(0, -1, -1))
			{
				this->lights.f2p4 -= LESS;
				this->lights.f2p1 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, -1, -1))
				this->lights.f2p1 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, -1))
				this->lights.f2p2 -= LESS;
			if (!calcLightsIsTransparent(0, 1, -1))
			{
				this->lights.f2p2 -= LESS;
				this->lights.f2p3 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 1, -1))
				this->lights.f2p3 -= LESS;
			if (!calcLightsIsTransparent(1, 0, -1))
			{
				this->lights.f2p3 -= LESS;
				this->lights.f2p4 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 0, -1))
			{
				this->lights.f2p1 -= LESS;
				this->lights.f2p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_LEFT)
		{
			if (!calcLightsIsTransparent(-1, -1, 1))
				this->lights.f3p4 -= LESS;
			if (!calcLightsIsTransparent(-1, -1, 0))
			{
				this->lights.f3p4 -= LESS;
				this->lights.f3p1 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, -1, -1))
				this->lights.f3p1 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, -1))
				this->lights.f3p2 -= LESS;
			if (!calcLightsIsTransparent(-1, 1, 0))
			{
				this->lights.f3p2 -= LESS;
				this->lights.f3p3 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 1, 1))
				this->lights.f3p3 -= LESS;
			if (!calcLightsIsTransparent(-1, 0, 1))
			{
				this->lights.f3p3 -= LESS;
				this->lights.f3p4 -= LESS;
			}
			if (!calcLightsIsTransparent(-1, 0, -1))
			{
				this->lights.f3p1 -= LESS;
				this->lights.f3p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_RIGHT)
		{
			if (!calcLightsIsTransparent(1, -1, 1))
				this->lights.f4p4 -= LESS;
			if (!calcLightsIsTransparent(1, -1, 0))
			{
				this->lights.f4p4 -= LESS;
				this->lights.f4p1 -= LESS;
			}
			if (!calcLightsIsTransparent(1, -1, -1))
				this->lights.f4p1 -= LESS;
			if (!calcLightsIsTransparent(1, 1, -1))
				this->lights.f4p2 -= LESS;
			if (!calcLightsIsTransparent(1, 1, 0))
			{
				this->lights.f4p2 -= LESS;
				this->lights.f4p3 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 1, 1))
				this->lights.f4p3 -= LESS;
			if (!calcLightsIsTransparent(1, 0, 1))
			{
				this->lights.f4p3 -= LESS;
				this->lights.f4p4 -= LESS;
			}
			if (!calcLightsIsTransparent(1, 0, -1))
			{
				this->lights.f4p1 -= LESS;
				this->lights.f4p2 -= LESS;
			}
		}
		if (this->visibleFaces & BLOCK_FACE_UP)
		{
			if (this->y < CHUNK_HEIGHT - 1)
			{
				if (!calcLightsIsTransparent(-1, 1, -1))
					this->lights.f5p2 -= LESS;
				if (!calcLightsIsTransparent(-1, 1, 0))
				{
					this->lights.f5p1 -= LESS;
					this->lights.f5p2 -= LESS;
				}
				if (!calcLightsIsTransparent(-1, 1, 1))
					this->lights.f5p1 -= LESS;
				if (!calcLightsIsTransparent(1, 1, -1))
					this->lights.f5p3 -= LESS;
				if (!calcLightsIsTransparent(1, 1, 0))
				{
					this->lights.f5p3 -= LESS;
					this->lights.f5p4 -= LESS;
				}
				if (!calcLightsIsTransparent(1, 1, 1))
					this->lights.f5p4 -= LESS;
				if (!calcLightsIsTransparent(0, 1, -1))
				{
					this->lights.f5p2 -= LESS;
					this->lights.f5p3 -= LESS;
				}
				if (!calcLightsIsTransparent(0, 1, 1))
				{
					this->lights.f5p4 -= LESS;
					this->lights.f5p1 -= LESS;
				}
			}
		}
		if (this->visibleFaces & BLOCK_FACE_DOWN)
		{
			if (this->y < 0)
			{
				if (!calcLightsIsTransparent(-1, -1, 1))
					this->lights.f6p1 -= LESS;
				if (!calcLightsIsTransparent(-1, -1, 0))
				{
					this->lights.f6p1 -= LESS;
					this->lights.f6p2 -= LESS;
				}
				if (!calcLightsIsTransparent(-1, -1, -1))
					this->lights.f6p2 -= LESS;
				if (!calcLightsIsTransparent(1, -1, -1))
					this->lights.f6p3 -= LESS;
				if (!calcLightsIsTransparent(1, -1, 0))
				{
					this->lights.f6p3 -= LESS;
					this->lights.f6p4 -= LESS;
				}
				if (!calcLightsIsTransparent(1, -1, 1))
					this->lights.f6p4 -= LESS;
				if (!calcLightsIsTransparent(0, -1, -1))
				{
					this->lights.f6p2 -= LESS;
					this->lights.f6p3 -= LESS;
				}
				if (!calcLightsIsTransparent(0, -1, 1))
				{
					this->lights.f6p4 -= LESS;
					this->lights.f6p1 -= LESS;
				}
			}
		}
	}

}
