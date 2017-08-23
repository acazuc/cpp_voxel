#include "Chunk.h"
#include "World.h"
#include "Main.h"
#include <iostream>

namespace voxel
{

	Chunk::Chunk(World &world, int32_t x, int32_t z)
	: texCoordsBuffer(NULL)
	, vertexesBuffer(NULL)
	, indicesBuffer(NULL)
	, colorsBuffer(NULL)
	, world(world)
	, x(x)
	, z(z)
	, mustGenerateBuffers(true)
	{
		if ((this->chunkXLess = this->world.getChunk(this->x - CHUNK_WIDTH, this->z)))
			this->chunkXLess->setChunkXMore(this);
		if ((this->chunkXMore = this->world.getChunk(this->x + CHUNK_WIDTH, this->z)))
			this->chunkXMore->setChunkXLess(this);
		if ((this->chunkZLess = this->world.getChunk(this->x, this->z - CHUNK_WIDTH)))
			this->chunkZLess->setChunkZMore(this);
		if ((this->chunkZMore = this->world.getChunk(this->x, this->z + CHUNK_WIDTH)))
			this->chunkZMore->setChunkZLess(this);
		this->blocks = new Block*[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];
		for (uint32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (uint32_t y = 0; y < CHUNK_HEIGHT; ++y)
			{
				for (uint32_t z = 0; z < CHUNK_WIDTH; ++z)
				{
					uint32_t noiseIndex = (this->world.getNoise().get2(this->x + x, this->z + z)) * CHUNK_HEIGHT / 3  + CHUNK_HEIGHT / 3;
					if (y > noiseIndex)
					{
						this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = NULL;
						continue;
					}
					uint8_t blockType = 1;
					if (y < CHUNK_HEIGHT * 1 / 4)
						blockType = 1;
					else if (y == CHUNK_HEIGHT * 1 / 4)
						blockType = 2;
					else
						blockType = 3;
					this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = new Block(this, this->x + x, y, this->z + z, blockType);
				}
			}
		}
	}

	Chunk::~Chunk()
	{
		for (uint32_t i = 0; i < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH; ++i)
			delete (this->blocks[i]);
		delete[] (this->blocks);
		delete (this->texCoordsBuffer);
		delete (this->vertexesBuffer);
		delete (this->indicesBuffer);
		delete (this->colorsBuffer);
		if (this->chunkXLess)
			this->chunkXLess->setChunkXMore(NULL);
		if (this->chunkXMore)
			this->chunkXMore->setChunkXLess(NULL);
		if (this->chunkZLess)
			this->chunkZLess->setChunkZMore(NULL);
		if (this->chunkZMore)
			this->chunkZMore->setChunkZLess(NULL);
	}

	void Chunk::draw()
	{
		if (!this->world.getFrustum().check(this->x, 0, this->z, this->x + CHUNK_WIDTH, CHUNK_HEIGHT, this->z + CHUNK_WIDTH))
			return;
		if (this->mustGenerateBuffers)
		{
			this->mustGenerateBuffers = false;
			generateGLBuffer();
		}
		Main::getTexCoordsLocation()->setDataBuffer(*this->texCoordsBuffer);
		Main::getVertexesLocation()->setDataBuffer(*this->vertexesBuffer);
		Main::getColorsLocation()->setDataBuffer(*this->colorsBuffer);
		this->indicesBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_INT, (void*)0);
	}

	void Chunk::generateGLBuffer()
	{
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		std::vector<GLuint> indices;
		for (uint32_t i = 0; i < CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH; ++i)
		{
			Block *block = this->blocks[i];
			if (block)
				block->fillBuffers(vertexes, texCoords, colors, indices);
		}
		if (!this->texCoordsBuffer)
			this->texCoordsBuffer = new DataBuffer();
		if (!this->vertexesBuffer)
			this->vertexesBuffer = new DataBuffer();
		if (!this->indicesBuffer)
			this->indicesBuffer = new DataBuffer();
		if (!this->colorsBuffer)
			this->colorsBuffer = new DataBuffer();
		this->texCoordsBuffer->setData(GL_ARRAY_BUFFER, texCoords.data(), texCoords.size() * sizeof(glm::vec2), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->vertexesBuffer->setData(GL_ARRAY_BUFFER, vertexes.data(), vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->indicesBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
		this->colorsBuffer->setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->verticesNb = indices.size();
	}

	void Chunk::setChunkXLess(Chunk *chunk)
	{
		this->chunkXLess = chunk;
		this->mustGenerateBuffers = true;
	}

	void Chunk::setChunkXMore(Chunk *chunk)
	{
		this->chunkXMore = chunk;
		this->mustGenerateBuffers = true;
	}

	void Chunk::setChunkZLess(Chunk *chunk)
	{
		this->chunkZLess = chunk;
		this->mustGenerateBuffers = true;
	}

	void Chunk::setChunkZMore(Chunk *chunk)
	{
		this->chunkZMore = chunk;
		this->mustGenerateBuffers = true;
	}

}
