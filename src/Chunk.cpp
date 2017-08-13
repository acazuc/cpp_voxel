#include "Chunk.h"
#include "World.h"
#include "Main.h"
#include <iostream>

namespace voxel
{

static void GLErrors(std::string err)
{
	GLenum glErr;
	while ((glErr = glGetError()) != GL_NO_ERROR)
	{
		std::cout << err << ": ";
		if (glErr == GL_INVALID_ENUM)
			std::cout << "GL_INVALID_ENUM" << std::endl;
		else if (glErr == GL_INVALID_VALUE)
			std::cout << "GL_INVALID_VALUE" << std::endl;
		else if (glErr == GL_INVALID_OPERATION)
			std::cout << "GL_INVALID_OPERATION" << std::endl;
		else if (glErr == GL_STACK_OVERFLOW)
			std::cout << "GL_STACK_OVERFLOW" << std::endl;
		else if (glErr == GL_STACK_UNDERFLOW)
			std::cout << "GL_STACK_UNDERFLOW" << std::endl;
		else if (glErr == GL_OUT_OF_MEMORY)
			std::cout << "GL_OUT_OF_MEMORY" << std::endl;
	}
}

	Chunk::Chunk(World *world, int32_t x, int32_t y)
	: world(world)
	, x(x)
	, y(y)
	{
		this->blocks = new Block***[CHUNK_WIDTH];
		for (uint8_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			this->blocks[x] = new Block**[CHUNK_HEIGHT];
			for (uint8_t y = 0; y < CHUNK_HEIGHT; ++y)
			{
				this->blocks[x][y] = new Block*[CHUNK_WIDTH];
				for (uint8_t z = 0; z < CHUNK_WIDTH; ++z)
				{
					this->blocks[x][y][z] = new Block(this->x + x, y, this->y + z, 1);
				}
			}
		}
		generateGLBuffer();
	}

	void Chunk::draw()
	{
		Main::getTexCoordsLocation()->setDataBuffer(this->texCoordsBuffer);
		GLErrors("Chunk::draw 1");
		Main::getVertexesLocation()->setDataBuffer(this->vertexesBuffer);
		GLErrors("Chunk::draw 2");
		Main::getColorsLocation()->setDataBuffer(this->colorsBuffer);
		GLErrors("Chunk::draw 3");
		this->indicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		GLErrors("Chunk::draw 4");
		glm::mat4 mvp = Main::getProjMat() * this->world->getPlayer().getViewMat();
		Main::getMvpLocation()->setMat4f(mvp);
		GLErrors("Chunk::draw 5");
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_INT, (void*)0);
		GLErrors("Chunk::draw 6");
	}

	void Chunk::generateGLBuffer()
	{
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> vertexes;
		std::vector<GLuint> indices;
		std::vector<glm::vec3> colors;
		for (uint8_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (uint8_t y = 0; y < CHUNK_HEIGHT; ++y)
			{
				for (uint8_t z = 0; z < CHUNK_WIDTH; ++z)
				{
					Block *block = this->blocks[x][y][z];
					if (block)
						block->fillBuffers(vertexes, texCoords, colors, indices);
				}
			}
		}
		this->texCoordsBuffer.setData(GL_ARRAY_BUFFER, texCoords.data(), texCoords.size() * sizeof(glm::vec2), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		GLErrors("Chunk::gl 1");
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes.data(), vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		GLErrors("Chunk::gl 2");
		this->indicesBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
		GLErrors("Chunk::gl 3");
		this->colorsBuffer.setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		GLErrors("Chunk::gl 4");
		this->verticesNb = texCoords.size();
	}

}
