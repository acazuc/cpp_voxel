#include "Chunk.h"
#include "Noise/WorleyNoise.h"
#include "World.h"
#include "Main.h"
#include <cstring>

namespace voxel
{

	uint8_t Chunk::availableRebuilds;

	Chunk::Chunk(World &world, int32_t x, int32_t z)
	: texCoordsBuffer(NULL)
	, vertexesBuffer(NULL)
	, indicesBuffer(NULL)
	, colorsBuffer(NULL)
	, world(world)
	, aabb(glm::vec3(x, 0, z), glm::vec3(x + CHUNK_WIDTH, CHUNK_HEIGHT, z + CHUNK_WIDTH))
	, x(x)
	, z(z)
	, mustGenerateLightMap(true)
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
		this->topBlocks = new uint8_t[CHUNK_WIDTH * CHUNK_WIDTH];
		this->blocks = new ChunkBlock[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];
		this->lightMap = new uint8_t[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];
		std::memset(this->lightMap, 0, CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH);
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				//float noiseIndex = 0;
				float noiseIndex = this->world.getNoise().get2((this->x + x), (this->z + z));
				//float noiseIndex = std::min(1., std::max(-1., WorleyNoise::get2((this->x + x) / 50., (this->z + z) / 50.)));
				//noiseIndex *= this->world.getNoise().get2(this->x + x, this->z + z);
				//float noiseIndex = this->world.getNoise().get3(this->x + x, this->z + z, 400) / 2;
				//noiseIndex += this->world.getNoise().get3(this->x + x, this->z + z, 3) / 3;
				//noiseIndex += this->world.getNoise().get3(this->x + x, this->z + z, 300000) / 4;
				noiseIndex = noiseIndex * CHUNK_HEIGHT / 6 + CHUNK_HEIGHT / 4;
				noiseIndex = std::round(noiseIndex);
				this->topBlocks[getXZId(x, z)] = noiseIndex;
				for (int32_t y = 0; y < CHUNK_HEIGHT; ++y)
				{
					if (y > noiseIndex + 1)
					{
						this->lightMap[getXYZId(glm::vec3(x, y, z))] = 0xf;
						continue;
					}
					uint8_t blockType = 1;
					if (y == 0)
						blockType = 7;
					else if (y == noiseIndex)
						blockType = 2;
					else if (y == noiseIndex + 1)
						blockType = 6;
					else if (y > noiseIndex - 3)
						blockType = 3;
					this->blocks[getXYZId(glm::vec3(x, y, z))].setType(blockType);
				}
			}
		}
		generateLightMap();
	}

	Chunk::~Chunk()
	{
		delete[] (this->topBlocks);
		delete[] (this->lightMap);
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
		if (!this->world.getFrustum().check(this->aabb))
			return;
		if (availableRebuilds > 0)
		{
			if (this->mustGenerateLightMap)
			{
				availableRebuilds--;
				generateLightMap();
				generateGLBuffer();
			}
			if (this->mustGenerateBuffers)
			{
				availableRebuilds--;
				generateGLBuffer();
			}
		}
		if (!this->texCoordsBuffer)
			return;
		Main::getBlocksShader().texCoordsLocation->setVertexBuffer(*this->texCoordsBuffer);
		Main::getBlocksShader().vertexesLocation->setVertexBuffer(*this->vertexesBuffer);
		Main::getBlocksShader().colorsLocation->setVertexBuffer(*this->colorsBuffer);
		this->indicesBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_INT, (void*)0);
	}

	void Chunk::setBlockLightRec(glm::vec3 pos, uint8_t light)
	{
		if (pos.y >= this->topBlocks[getXZId(pos.x, pos.z)])
		{
			light = std::max(light, uint8_t(0xf));
		}
		else
		{
			if (pos.x > 0)
			{
				if (pos.y > this->topBlocks[getXZId(pos.x - 1, pos.z)])
				{
					light = std::max(light, uint8_t(0xf));
					goto endNearTop;
				}
			}
			else
			{
				if (this->chunkXLess)
				{
					if (pos.y > this->chunkXLess->getTopBlockAt(CHUNK_WIDTH - 1, pos.z))
					{
						light = std::max(light, uint8_t(0xf));
						goto endNearTop;
					}
				}
				else
				{
					light = 0;
					goto endNearTop;
				}
			}
			if (pos.x < CHUNK_WIDTH - 1)
			{
				if (pos.y > this->topBlocks[getXZId(pos.x + 1, pos.z)])
				{
					light = std::max(light, uint8_t(0xf));
					goto endNearTop;
				}
			}
			else
			{
				if (this->chunkXMore)
				{
					if (pos.y > this->chunkXMore->getTopBlockAt(0, pos.z))
					{
						light = std::max(light, uint8_t(0xf));
						goto endNearTop;
					}
				}
				else
				{
					light = 0;
					goto endNearTop;
				}
			}
			if (pos.z > 0)
			{
				if (pos.y > this->topBlocks[getXZId(pos.x, pos.z - 1)])
				{
					light = std::max(light, uint8_t(0xf));
					goto endNearTop;
				}
			}
			else
			{
				if (this->chunkZLess)
				{
					if (pos.y > this->chunkZLess->getTopBlockAt(pos.x, CHUNK_WIDTH - 1))
					{
						light = std::max(light, uint8_t(0xf));
						goto endNearTop;
					}
				}
				else
				{
					light = 0;
					goto endNearTop;
				}
			}
			if (pos.z < CHUNK_WIDTH - 1)
			{
				if (pos.y > this->topBlocks[getXZId(pos.x, pos.z + 1)])
				{
					light = std::max(light, uint8_t(0xf));
					goto endNearTop;
				}
			}
			else
			{
				if (this->chunkZMore)
				{
					if (pos.y > this->chunkZMore->getTopBlockAt(pos.x, 0))
					{
						light = std::max(light, uint8_t(0xf));
						goto endNearTop;
					}
				}
				else
				{
					light = 0;
					goto endNearTop;
				}
			}
		}
	endNearTop:
		if (pos.x == 0 && this->chunkXLess)
		{
			uint8_t nearLight = this->chunkXLess->getLightAt(glm::vec3(CHUNK_WIDTH - 1, pos.y, pos.z));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		else if (pos.x == CHUNK_WIDTH - 1 && this->chunkXMore)
		{
			uint8_t nearLight = this->chunkXMore->getLightAt(glm::vec3(0, pos.y, pos.z));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		if (pos.z == 0 && this->chunkZLess)
		{
			uint8_t nearLight = this->chunkZLess->getLightAt(glm::vec3(pos.x, pos.y, CHUNK_WIDTH - 1));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		else if (pos.z == CHUNK_WIDTH - 1 && this->chunkZMore)
		{
			uint8_t nearLight = this->chunkZMore->getLightAt(glm::vec3(pos.x, pos.y, 0));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		uint8_t curLvl = this->lightMap[getXYZId(pos)];
		if (curLvl >= light)
			return;
		this->lightMap[getXYZId(pos)] = light;
		if (light <= 1)
			return;
		if (!this->blocks[getXYZId(pos)].isTransparent())
		{
			if (pos.x > 0)
				setBlockLightRec(glm::vec3(pos.x - 1, pos.y, pos.z), 0);
			if (pos.x < CHUNK_WIDTH - 1)
				setBlockLightRec(glm::vec3(pos.x + 1, pos.y, pos.z), 0);
			if (pos.y > 0)
				setBlockLightRec(glm::vec3(pos.x, pos.y - 1, pos.z), 0);
			if (pos.y < CHUNK_HEIGHT - 1)
				setBlockLightRec(glm::vec3(pos.x, pos.y + 1, pos.z), 0);
			if (pos.z > 0)
				setBlockLightRec(glm::vec3(pos.x, pos.y, pos.z - 1), 0);
			if (pos.z < CHUNK_WIDTH - 1)
				setBlockLightRec(glm::vec3(pos.x, pos.y, pos.z + 1), 0);
			return;
		}
		if (pos.x > 0)
			setBlockLightRec(glm::vec3(pos.x - 1, pos.y, pos.z), light - 1);
		if (pos.x < CHUNK_WIDTH - 1)
			setBlockLightRec(glm::vec3(pos.x + 1, pos.y, pos.z), light - 1);
		if (pos.y > 0)
			setBlockLightRec(glm::vec3(pos.x, pos.y - 1, pos.z), light - 1);
		if (pos.y < CHUNK_HEIGHT - 1 && pos.y < this->topBlocks[getXZId(pos.x, pos.z)])
			setBlockLightRec(glm::vec3(pos.x, pos.y + 1, pos.z), light - 1);
		if (pos.z > 0)
			setBlockLightRec(glm::vec3(pos.x, pos.y, pos.z - 1), light - 1);
		if (pos.z < CHUNK_WIDTH - 1)
			setBlockLightRec(glm::vec3(pos.x, pos.y, pos.z + 1), light - 1);
	}

	void Chunk::generateLightMap()
	{
		this->mustGenerateLightMap = false;
		std::memset(this->lightMap, 0, CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH * sizeof(*this->lightMap));
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				int16_t topBlock = this->topBlocks[x * CHUNK_WIDTH + z];
				setBlockLightRec(glm::vec3(x, topBlock, z), 0xf);
			}
		}
	}

	void Chunk::generateGLBuffer()
	{
		this->mustGenerateBuffers = false;
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
					ChunkBlock *block = &this->blocks[getXYZId(glm::vec3(x, y, z))];
					pos.x = this->x + x;
					pos.y = y;
					pos.z = this->z + z;
					block->fillBuffers(this, pos, vertexes, texCoords, colors, indices);
				}
			}
		}
		if (!this->texCoordsBuffer)
			this->texCoordsBuffer = new VertexBuffer();
		if (!this->vertexesBuffer)
			this->vertexesBuffer = new VertexBuffer();
		if (!this->indicesBuffer)
			this->indicesBuffer = new VertexBuffer();
		if (!this->colorsBuffer)
			this->colorsBuffer = new VertexBuffer();
		this->texCoordsBuffer->setData(GL_ARRAY_BUFFER, texCoords.data(), texCoords.size() * sizeof(glm::vec2), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->vertexesBuffer->setData(GL_ARRAY_BUFFER, vertexes.data(), vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->indicesBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
		this->colorsBuffer->setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->verticesNb = indices.size();
	}

	void Chunk::addBlock(glm::vec3 pos, uint8_t type)
	{
		if (pos.x == 0)
		{
			if (this->chunkXLess)
				this->chunkXLess->regenerateLightMap();
		}
		else if (pos.x == CHUNK_WIDTH - 1)
		{
			if (this->chunkXMore)
				this->chunkXMore->regenerateLightMap();
		}
		if (pos.z == 0)
		{
			if (this->chunkZLess)
				this->chunkZLess->regenerateLightMap();
		}
		else if (pos.z == CHUNK_WIDTH - 1)
		{
			if (this->chunkZMore)
				this->chunkZMore->regenerateLightMap();
		}
		if (pos.y > this->topBlocks[getXZId(x, pos.z)])
			this->topBlocks[getXZId(pos.x, pos.z)] = pos.y;
		this->blocks[getXYZId(pos)].setType(type);
		regenerateLightMap();
	}

	void Chunk::destroyBlock(glm::vec3 pos)
	{
		if (pos.x == 0)
		{
			if (this->chunkXLess)
				this->chunkXLess->regenerateLightMap();
		}
		else if (pos.x == CHUNK_WIDTH - 1)
		{
			if (this->chunkXMore)
				this->chunkXMore->regenerateLightMap();
		}
		if (pos.z == 0)
		{
			if (this->chunkZLess)
				this->chunkZLess->regenerateLightMap();
		}
		else if (pos.z == CHUNK_WIDTH - 1)
		{
			if (this->chunkZMore)
				this->chunkZMore->regenerateLightMap();
		}
		if (pos.y == this->topBlocks[getXZId(pos.x, pos.z)])
		{
			for (int32_t i = CHUNK_HEIGHT - 1; i >= 0; --i)
			{
				ChunkBlock *block = getBlockAt(glm::vec3(pos.x, i, pos.z));
				if (block->getType())
				{
					if (i == pos.y)
						continue;
					this->topBlocks[getXZId(pos.x, pos.z)] = i;
					break;
				}
			}
		}
		this->blocks[getXYZId(pos)].setType(0);
		regenerateLightMap();
	}

	void Chunk::setChunkXLess(Chunk *chunk)
	{
		this->chunkXLess = chunk;
		regenerateLightMap();
	}

	void Chunk::setChunkXMore(Chunk *chunk)
	{
		this->chunkXMore = chunk;
		regenerateLightMap();
	}

	void Chunk::setChunkZLess(Chunk *chunk)
	{
		this->chunkZLess = chunk;
		regenerateLightMap();
	}

	void Chunk::setChunkZMore(Chunk *chunk)
	{
		this->chunkZMore = chunk;
		regenerateLightMap();
	}

}
