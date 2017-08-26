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
		glm::vec3 color(1, 1, 1);
		float f1p1 = lights.f1p1 / 16. * .5 + .25;
		float f1p2 = lights.f1p2 / 16. * .5 + .25;
		float f1p3 = lights.f1p3 / 16. * .5 + .25;
		float f1p4 = lights.f1p4 / 16. * .5 + .25;
		float f2p1 = lights.f2p1 / 16. * .5 + .25;
		float f2p2 = lights.f2p2 / 16. * .5 + .25;
		float f2p3 = lights.f2p3 / 16. * .5 + .25;
		float f2p4 = lights.f2p4 / 16. * .5 + .25;
		float f3p1 = lights.f3p1 / 16. * .5 + .25;
		float f3p2 = lights.f3p2 / 16. * .5 + .25;
		float f3p3 = lights.f3p3 / 16. * .5 + .25;
		float f3p4 = lights.f3p4 / 16. * .5 + .25;
		float f4p1 = lights.f4p1 / 16. * .5 + .25;
		float f4p2 = lights.f4p2 / 16. * .5 + .25;
		float f4p3 = lights.f4p3 / 16. * .5 + .25;
		float f4p4 = lights.f4p4 / 16. * .5 + .25;
		float f5p1 = lights.f5p1 / 16. * .5 + .25;
		float f5p2 = lights.f5p2 / 16. * .5 + .25;
		float f5p3 = lights.f5p3 / 16. * .5 + .25;
		float f5p4 = lights.f5p4 / 16. * .5 + .25;
		float f6p1 = lights.f6p1 / 16. * .5 + .25;
		float f6p2 = lights.f6p2 / 16. * .5 + .25;
		float f6p3 = lights.f6p3 / 16. * .5 + .25;
		float f6p4 = lights.f6p4 / 16. * .5 + .25;
		float texX = ((this->type - 1) % 16) / 16.;
		float texY = ((this->type - 1) / 16) / 16.;
		if (this->type == 1)
		{
			texX = 3 / 16.;
			texY = 0 / 16.;
		}
		if (this->visibleFaces & BLOCK_FACE_FRONT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f1p1 * color.x, f1p1 * color.y, f1p1 * color.z));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f1p2 * color.x, f1p2 * color.y, f1p2 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f1p3 * color.x, f1p3 * color.y, f1p3 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
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
		if (this->visibleFaces & BLOCK_FACE_BACK)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f2p1 * color.x, f2p1 * color.y, f2p1 * color.z));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f2p2 * color.x, f2p2 * color.y, f2p2 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f2p3 * color.x, f2p3 * color.y, f2p3 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
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
		if (this->visibleFaces & BLOCK_FACE_LEFT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			colors.push_back(glm::vec3(f3p1 * color.x, f3p1 * color.y, f3p1 * color.z));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f3p2 * color.x, f3p2 * color.y, f3p2 * color.z));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f3p3 * color.x, f3p3 * color.y, f3p3 * color.z));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
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
		if (this->visibleFaces & BLOCK_FACE_RIGHT)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f4p1 * color.x, f4p1 * color.y, f4p1 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY));
			colors.push_back(glm::vec3(f4p2 * color.x, f4p2 * color.y, f4p2 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f4p3 * color.x, f4p3 * color.y, f4p3 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
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
		if (this->visibleFaces & BLOCK_FACE_UP)
		{
			float oldTexX = texX;
			float oldTexY = texY;
			if (this->type == 1)
			{
				texX = 0;
				texY = 0;
			}
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f5p1 * color.x, f5p1 * color.y, f5p1 * color.z));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f5p2 * color.x, f5p2 * color.y, f5p2 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f5p3 * color.x, f5p3 * color.y, f5p3 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
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
		if (this->visibleFaces & BLOCK_FACE_DOWN)
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY));
			colors.push_back(glm::vec3(f6p1 * color.x, f6p1 * color.y, f6p1 * color.z));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX, texY + 1. / 16));
			colors.push_back(glm::vec3(f6p2 * color.x, f6p2 * color.y, f6p2 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(texX + 1. / 16, texY + 1. / 16));
			colors.push_back(glm::vec3(f6p3 * color.x, f6p3 * color.y, f6p3 * color.z));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
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
				if (!block || (block->isTransparent() && block->getType() != this->type))
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
				if (!block || (block->isTransparent() && block->getType() != this->type))
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
				if (!block || (block->isTransparent() && block->getType() != this->type))
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
				if (!block || (block->isTransparent() && block->getType() != this->type))
					this->visibleFaces |= BLOCK_FACE_RIGHT;
			}
		}
		else
		{
			Block *block = this->chunk->getBlockAt(this->x - this->chunk->getX() + 1, this->y, this->z - this->chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				this->visibleFaces |= BLOCK_FACE_RIGHT;
		}
		if (this->y == CHUNK_HEIGHT - 1)
		{
			this->visibleFaces |= BLOCK_FACE_UP;
		}
		else
		{
			Block *block = this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y + 1, this->z - this->chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
				this->visibleFaces |= BLOCK_FACE_UP;
		}
		if (this->y == 0)
		{
			this->visibleFaces |= BLOCK_FACE_DOWN;
		}
		else
		{
			Block * block = this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y - 1, this->z - this->chunk->getZ());
			if (!block || (block->isTransparent() && block->getType() != this->type))
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
