#include "Block.h"

#define BLOCK_SIZE .5

namespace voxel
{

	Block::Block(int32_t x, int32_t y, int32_t z, uint8_t type)
	: type(type)
	, x(x)
	, y(y)
	, z(z)
	{
		//Empty
	}

	void Block::fillBuffers(std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices)
	{
		if (isVisibleFace(BLOCK_FACE_FRONT))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
		}
		if (isVisibleFace(BLOCK_FACE_BACK))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 2);
		}
		if (isVisibleFace(BLOCK_FACE_LEFT))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
		}
		if (isVisibleFace(BLOCK_FACE_RIGHT))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 2);
		}
		if (isVisibleFace(BLOCK_FACE_UP))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 4);
		}
		if (isVisibleFace(BLOCK_FACE_DOWN))
		{
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(0, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 1));
			colors.push_back(glm::vec3(1, 1, 1));
			vertexes.push_back(glm::vec3(x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE));
			texCoords.push_back(glm::vec2(1, 0));
			colors.push_back(glm::vec3(1, 1, 1));
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 4);
			indices.push_back(currentIndice + 2);
		}
	}

	bool Block::isVisibleFace(enum BlockFace face)
	{
		switch (face)
		{
			case BLOCK_FACE_FRONT:
				break;
			case BLOCK_FACE_BACK:
				break;
			case BLOCK_FACE_LEFT:
				break;
			case BLOCK_FACE_RIGHT:
				break;
			case BLOCK_FACE_UP:
				break;
			case BLOCK_FACE_DOWN:
				break;
		}
		return (true);
	}

}
