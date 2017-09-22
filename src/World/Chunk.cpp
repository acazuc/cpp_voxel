#include "Chunk.h"
#include "Noise/WorleyNoise.h"
#include "ChunkTessellator.h"
#include "Blocks/Blocks.h"
#include "Utils/System.h"
#include "World.h"
#include "Debug.h"
#include "Main.h"
#include <cstring>

namespace voxel
{

	uint8_t Chunk::availableRebuilds;

	Chunk::Chunk(World &world, int32_t x, int32_t z)
	: world(world)
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
				this->topBlocks[getXZId(x, z)] = std::max(noiseIndex, CHUNK_HEIGHT / 4.f);
				for (int32_t y = 0; y < CHUNK_HEIGHT; ++y)
				{
					if (y > noiseIndex && y > CHUNK_HEIGHT / 4)
					{
						this->lightMap[getXYZId(glm::vec3(x, y, z))] = 0xf;
						continue;
					}
					uint8_t blockType = 1;
					if (y == 0)
						blockType = 7;
					else if (y == noiseIndex)
						blockType = 2;
					else if (y > noiseIndex)
						blockType = 8;
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
		if (this->chunkXLess)
			this->chunkXLess->setChunkXMore(NULL);
		if (this->chunkXMore)
			this->chunkXMore->setChunkXLess(NULL);
		if (this->chunkZLess)
			this->chunkZLess->setChunkZMore(NULL);
		if (this->chunkZMore)
			this->chunkZMore->setChunkZLess(NULL);
	}

	void Chunk::tick()
	{
		//
	}

	void Chunk::draw(uint8_t layer)
	{
		if (layer == 0)
			this->visible = this->world.getFrustum().check(this->aabb);
		if (!this->visible)
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
		if (!this->layers[layer].texCoordsBuffer || !this->layers[layer].verticesNb)
			return;
		Main::getBlocksShader().texCoordsLocation->setVertexBuffer(*this->layers[layer].texCoordsBuffer);
		Main::getBlocksShader().vertexesLocation->setVertexBuffer(*this->layers[layer].vertexesBuffer);
		Main::getBlocksShader().colorsLocation->setVertexBuffer(*this->layers[layer].colorsBuffer);
		this->layers[layer].indicesBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, this->layers[layer].verticesNb, GL_UNSIGNED_INT, (void*)0);
	}

	void Chunk::setBlockLightRec(glm::vec3 pos, uint8_t light)
	{
		if (pos.y > this->topBlocks[getXZId(pos.x, pos.z)])
		{
			light = std::max(light, uint8_t(0xf));
			goto endNearTop;
		}
		else if (pos.y == this->topBlocks[getXZId(pos.x, pos.z)])
		{
			light = std::max(light, uint8_t(0xe));
			goto endNearTop;
		}
		if (pos.x > 0)
		{
			if (pos.y > this->topBlocks[getXZId(pos.x - 1, pos.z)])
			{
				light = std::max(light, uint8_t(0xe));
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkXLess)
			{
				if (pos.y > this->chunkXLess->getTopBlockAt(CHUNK_WIDTH - 1, pos.z))
				{
					light = std::max(light, uint8_t(0xe));
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
				light = std::max(light, uint8_t(0xe));
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkXMore)
			{
				if (pos.y > this->chunkXMore->getTopBlockAt(0, pos.z))
				{
					light = std::max(light, uint8_t(0xe));
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
				light = std::max(light, uint8_t(0xe));
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkZLess)
			{
				if (pos.y > this->chunkZLess->getTopBlockAt(pos.x, CHUNK_WIDTH - 1))
				{
					light = std::max(light, uint8_t(0xe));
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
				light = std::max(light, uint8_t(0xe));
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkZMore)
			{
				if (pos.y > this->chunkZMore->getTopBlockAt(pos.x, 0))
				{
					light = std::max(light, uint8_t(0xe));
					goto endNearTop;
				}
			}
			else
			{
				light = 0;
				goto endNearTop;
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
		if (this->blocks[getXYZId(pos)].getType())
		{
			Block *block = Blocks::getBlock(this->blocks[getXYZId(pos)].getType());
			if (block)
			{
				if (block->getOpacity() > light)
					return;
				light -= block->getOpacity();
			}
		}
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
				int16_t topBlock = this->topBlocks[getXZId(x, z)];
				setBlockLightRec(glm::vec3(x, topBlock, z), 0xf);
			}
		}
	}

	void Chunk::generateGLBuffer()
	{
		this->mustGenerateBuffers = false;
		for (uint8_t layer = 0; layer < 3; ++layer)
		{
			ChunkTessellator tessellator;
			glm::vec3 pos(1);
			for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
			{
				for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
				{
					int32_t topBlock = this->topBlocks[getXZId(x, z)];
					for (int32_t y = 0; y <= topBlock; ++y)
					{
						ChunkBlock *block = &this->blocks[getXYZId(glm::vec3(x, y, z))];
						pos.x = this->x + x;
						pos.y = y;
						pos.z = this->z + z;
						block->fillBuffers(this, pos, tessellator, layer);
					}
				}
			}
			if (!this->layers[layer].texCoordsBuffer)
				this->layers[layer].texCoordsBuffer = new VertexBuffer();
			if (!this->layers[layer].vertexesBuffer)
				this->layers[layer].vertexesBuffer = new VertexBuffer();
			if (!this->layers[layer].indicesBuffer)
				this->layers[layer].indicesBuffer = new VertexBuffer();
			if (!this->layers[layer].colorsBuffer)
				this->layers[layer].colorsBuffer = new VertexBuffer();
			this->layers[layer].texCoordsBuffer->setData(GL_ARRAY_BUFFER, tessellator.texCoords.data(), tessellator.texCoords.size() * sizeof(glm::vec2), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
			this->layers[layer].vertexesBuffer->setData(GL_ARRAY_BUFFER, tessellator.vertexes.data(), tessellator.vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
			this->layers[layer].indicesBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, tessellator.indices.data(), tessellator.indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
			this->layers[layer].colorsBuffer->setData(GL_ARRAY_BUFFER, tessellator.colors.data(), tessellator.colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
			this->layers[layer].verticesNb = tessellator.indices.size();
		}
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
		if (pos.y > this->topBlocks[getXZId(pos.x, pos.z)])
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
