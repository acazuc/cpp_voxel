#include "Block.h"
#include "Chunk.h"
#include "Debug.h"

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
		float rr = std::rand() / (float)RAND_MAX;
		float rg = std::rand() / (float)RAND_MAX;
		float rb = std::rand() / (float)RAND_MAX;
		if (isVisibleFace(BLOCK_FACE_FRONT))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 0);
		}
		if (isVisibleFace(BLOCK_FACE_BACK))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
		}
		if (isVisibleFace(BLOCK_FACE_LEFT))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 0);
		}
		if (isVisibleFace(BLOCK_FACE_RIGHT))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
		}
		if (isVisibleFace(BLOCK_FACE_UP))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
		}
		if (isVisibleFace(BLOCK_FACE_DOWN))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(rr, rg, rb));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(rr, rg, rb));
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
		}
	}

	bool Block::isVisibleFace(enum BlockFace face)
	{
		switch (face)
		{
			case BLOCK_FACE_FRONT:
				{
					if (this->z - this->chunk->getZ() == CHUNK_WIDTH - 1)
					{
						if (this->chunk->getChunkZMore())
							return (!this->chunk->getChunkZMore()->getBlockAt(this->x - this->chunk->getX(), this->y, 0)->isTransparent());
						return (true);
					}
					return (!this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y, this->z - this->chunk->getZ() + 1)->isTransparent());
				}
				break;
			case BLOCK_FACE_BACK:
				{
					if (this->z - this->chunk->getZ() == 0)
					{
						if (this->chunk->getChunkZLess())
							return (!this->chunk->getChunkZLess()->getBlockAt(this->x - this->chunk->getX(), this->y, CHUNK_WIDTH - 1)->isTransparent());
						return (true);
					}
					return (!this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y, this->z - this->chunk->getZ() - 1)->isTransparent());
				}
				break;
			case BLOCK_FACE_LEFT:
				{
					if (this->x - this->chunk->getX() == 0)
					{
						if (this->chunk->getChunkXLess())
							return (!this->chunk->getChunkXLess()->getBlockAt(CHUNK_WIDTH - 1, this->y, this->z - this->chunk->getZ())->isTransparent());
						return (true);
					}
					return (!this->chunk->getBlockAt(this->x - this->chunk->getX() - 1, this->y, this->z - this->chunk->getZ())->isTransparent());
				}
				break;
			case BLOCK_FACE_RIGHT:
				{
					if (this->x - this->chunk->getX() == CHUNK_WIDTH - 1)
					{
						if (this->chunk->getChunkXMore())
							return (!this->chunk->getChunkXMore()->getBlockAt(0, this->y, this->z - this->chunk->getZ())->isTransparent());
						return (true);
					}
					return (!this->chunk->getBlockAt(this->x - this->chunk->getX() + 1, this->y, this->z - this->chunk->getZ())->isTransparent());
				}
				break;
			case BLOCK_FACE_UP:
				{
					if (this->y == CHUNK_HEIGHT - 1)
						return (true);
					return (!this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y + 1, this->z - this->chunk->getZ())->isTransparent());
				}
				break;
			case BLOCK_FACE_DOWN:
				{
					if (this->y == 0)
						return (true);
					return (!this->chunk->getBlockAt(this->x - this->chunk->getX(), this->y - 1, this->z - this->chunk->getZ())->isTransparent());
				}
				break;
		}
		return (false);
	}

}
