#include "Chunk.h"
#include "World.h"
#include "Main.h"
#include <cstring>

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
	, deleted(false)
	{
		if ((this->chunkXLess = this->world.getChunk(this->x - CHUNK_WIDTH, this->z)))
			this->chunkXLess->setChunkXMore(this);
		if ((this->chunkXMore = this->world.getChunk(this->x + CHUNK_WIDTH, this->z)))
			this->chunkXMore->setChunkXLess(this);
		if ((this->chunkZLess = this->world.getChunk(this->x, this->z - CHUNK_WIDTH)))
			this->chunkZLess->setChunkZMore(this);
		if ((this->chunkZMore = this->world.getChunk(this->x, this->z + CHUNK_WIDTH)))
			this->chunkZMore->setChunkZLess(this);
		this->topBlocks = new uint16_t[CHUNK_WIDTH * CHUNK_WIDTH];
		this->blocks = new Block*[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];
		this->lightMap = new uint8_t[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				float noiseIndex = this->world.getNoise().get2(this->x + x, this->z + z);
				noiseIndex = noiseIndex * CHUNK_HEIGHT / 6 + CHUNK_HEIGHT / 4;
				noiseIndex = std::round(noiseIndex);
				for (int32_t y = 0; y < CHUNK_HEIGHT; ++y)
				{
					if (y > noiseIndex)
					{
						this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = NULL;
						this->lightMap[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = 0xf;
						continue;
					}
					uint8_t blockType = 1;
					if (y == 0)
						blockType = 18;
					else if (y == noiseIndex)
						blockType = 1;
					else if (y > noiseIndex - 3)
						blockType = 3;
					else
						blockType = 2;
					this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = new Block(blockType);
					this->topBlocks[x * CHUNK_WIDTH + z] = noiseIndex;
					if (y == noiseIndex)
						this->lightMap[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = 0xf;
					else
						this->lightMap[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = 0;
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
		Main::getBlocksShader().texCoordsLocation->setDataBuffer(*this->texCoordsBuffer);
		Main::getBlocksShader().vertexesLocation->setDataBuffer(*this->vertexesBuffer);
		Main::getBlocksShader().colorsLocation->setDataBuffer(*this->colorsBuffer);
		this->indicesBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_INT, (void*)0);
	}

	void Chunk::setBlockLightRec(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		uint8_t curLvl = this->lightMap[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z];
		if (curLvl >= light)
			return;
		this->lightMap[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = light;
		if (light <= 1)
			return;
		if (x > 0)
			setBlockLightRec(x - 1, y, z, light - 1);
		else if (this->chunkXLess)
			this->chunkXLess->setBlockLightRec(CHUNK_WIDTH, y, z, light - 1);
		if (x < CHUNK_WIDTH - 1)
			setBlockLightRec(x + 1, y, z, light - 1);
		else if (this->chunkXMore)
			this->chunkXMore->setBlockLightRec(0, y, z, light - 1);
		if (y > 0)
			setBlockLightRec(x, y - 1, z, light - 1);
		if (y < CHUNK_HEIGHT - 1)
			setBlockLightRec(x, y + 1, z, light - 1);
		if (z > 0)
			setBlockLightRec(x, y, z - 1, light - 1);
		else if (this->chunkZLess)
			this->chunkZLess->setBlockLightRec(x, y, CHUNK_WIDTH, light - 1);
		if (z < CHUNK_WIDTH - 1)
			setBlockLightRec(x, y, z + 1, light - 1);
		else if (this->chunkZMore)
			this->chunkZMore->setBlockLightRec(x, y, 0, light - 1);
	}

	void Chunk::generateLightMap()
	{
		std::memset(this->lightMap, 0, CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH * sizeof(*this->lightMap));
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				int16_t topBlock = this->topBlocks[x * CHUNK_WIDTH + z];
				setBlockLightRec(x, topBlock, z, 0xf);
			}
		}
	}

	void Chunk::generateGLBuffer()
	{
		generateLightMap();
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		std::vector<GLuint> indices;
		glm::vec3 pos(1);
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t y = 0; y < CHUNK_HEIGHT; ++y)
			{
				for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
				{
					Block *block = this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z];
					if (block)
					{
						pos.x = this->x + x;
						pos.y = y;
						pos.z = this->z + z;
						block->fillBuffers(this, pos, vertexes, texCoords, colors, indices);
					}
				}
			}
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

	void Chunk::destroyBlock(int32_t x, int32_t y, int32_t z)
	{
		if (x == 0)
		{
			if (this->chunkXLess)
				this->chunkXLess->regenerateBuffers();
		}
		else if (x == CHUNK_WIDTH - 1)
		{
			if (this->chunkXMore)
				this->chunkXMore->regenerateBuffers();
		}
		if (z == 0)
		{
			if (this->chunkZLess)
				this->chunkZLess->regenerateBuffers();
		}
		else if (z == CHUNK_WIDTH - 1)
		{
			if (this->chunkZMore)
				this->chunkZMore->regenerateBuffers();
		}
		if (y == this->topBlocks[x * CHUNK_WIDTH + z])
		{
			for (int32_t i = CHUNK_HEIGHT - 1; i >= 0; --i)
			{
				if (getBlockAt(x, i, z))
				{
					if (i == y)
						continue;
					this->topBlocks[x * CHUNK_WIDTH + z] = i;
					break;
				}
			}
		}
		delete (this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z]);
		this->blocks[(x * CHUNK_HEIGHT + y) * CHUNK_WIDTH + z] = NULL;
		this->mustGenerateBuffers = true;
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
