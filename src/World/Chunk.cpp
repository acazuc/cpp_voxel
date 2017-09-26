#include "Chunk.h"
#include "World/Biomes/Biomes.h"
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

	Chunk::Chunk(World &world, int32_t x, int32_t z)
	: world(world)
	, aabb(glm::vec3(x, 0, z), glm::vec3(x + CHUNK_WIDTH, CHUNK_HEIGHT, z + CHUNK_WIDTH))
	, x(x)
	, z(z)
	, mustGenerateLightMap(false)
	, mustGenerateBuffers(false)
	, mustUpdateBuffers(false)
	, generated(false)
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
		std::memset(this->topBlocks, 0, sizeof(this->topBlocks));
		std::memset(this->storages, 0, sizeof(this->storages));
		std::memset(this->biomes, 0, sizeof(this->biomes));
		if (this->chunkXLess)
			this->chunkXLess->regenerateLightMap();
		if (this->chunkXMore)
			this->chunkXMore->regenerateLightMap();
		if (this->chunkZLess)
			this->chunkZLess->regenerateLightMap();
		if (this->chunkZMore)
			this->chunkZMore->regenerateLightMap();
	}

	Chunk::~Chunk()
	{
		for (uint8_t i = 0; i < 16; ++i)
			delete (this->storages[i]);
		if (this->chunkXLess)
		{
			this->chunkXLess->setChunkXMore(NULL);
			this->chunkXLess->regenerateLightMap();
		}
		if (this->chunkXMore)
		{
			this->chunkXMore->setChunkXLess(NULL);
			this->chunkXMore->regenerateLightMap();
		}
		if (this->chunkZLess)
		{
			this->chunkZLess->setChunkZMore(NULL);
			this->chunkZLess->regenerateLightMap();
		}
		if (this->chunkZMore)
		{
			this->chunkZMore->setChunkZLess(NULL);
			this->chunkZMore->regenerateLightMap();
		}
	}

	void Chunk::moveGLBuffersToWorld()
	{
		for (uint8_t i = 0; i < 3; ++i)
		{
			ChunkLayer &layer = this->layers[i];
			if (layer.texCoordsBuffer)
				this->world.getBuffersToDelete().push_back(layer.texCoordsBuffer);
			if (layer.vertexesBuffer)
				this->world.getBuffersToDelete().push_back(layer.vertexesBuffer);
			if (layer.indicesBuffer)
				this->world.getBuffersToDelete().push_back(layer.indicesBuffer);
			if (layer.colorsBuffer)
				this->world.getBuffersToDelete().push_back(layer.colorsBuffer);
		}
	}

	void Chunk::generate()
	{
		if (this->generated)
			return;
		this->generated = true;
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				//float noiseIndex = -1;
				//float noiseIndex = this->world.getNoise().get2((this->x + x), (this->z + z));
				//float noiseIndex = std::min(1., std::max(-1., WorleyNoise::get2((this->x + x) / 50., (this->z + z) / 50.)));
				//noiseIndex *= this->world.getNoise().get2(this->x + x, this->z + z);
				//float noiseIndex = this->world.getNoise().get2(this->x + x, this->z + z) / 2;
				//noiseIndex += this->world.getNoise().get3(this->x + x, this->z + z, 3) / 3;
				//noiseIndex += this->world.getNoise().get3(this->x + x, this->z + z, 300000) / 4;
				float noiseIndex = this->world.getNoise().get2((this->x + x) * 100, (this->z + z) * 100);
				noiseIndex = noiseIndex * CHUNK_HEIGHT / 5 + CHUNK_HEIGHT / 4;
				noiseIndex = std::round(noiseIndex);
				for (int32_t y = 0; y < CHUNK_HEIGHT; ++y)
				{
					if (y > noiseIndex && y > CHUNK_HEIGHT / 4)
						continue;
					uint8_t blockType = 1;
					if (y == 0)
						blockType = 7;
					else if (y == noiseIndex)
					{
						if (y <= CHUNK_HEIGHT / 4)
							blockType = 12;
						else
							blockType = 2;
					}
					else if (y > noiseIndex)
						blockType = 8;
					else if (y > noiseIndex - 3)
					{
						if (y <= CHUNK_HEIGHT / 4)
							blockType = 12;
						else
							blockType = 3;
					}
					setBlockIfReplaceable(glm::vec3(x, y, z), blockType);
				}
			}
		}
		Biomes::getBiome(1)->generate(*this);
		generateLightMap();
	}

	void Chunk::tick()
	{
		//
	}

	void Chunk::draw(uint8_t layer)
	{
		if (!this->generated)
			return;
		if (layer == 0)
			this->visible = this->world.getFrustum().check(this->aabb);
		if (!this->visible)
			return;
		if (layer == 0)
		{
			if (this->mustUpdateBuffers)
				updateGLBuffers();
		}
		if (!this->layers[layer].verticesNb)
			return;
		Main::getBlocksShader().texCoordsLocation->setVertexBuffer(*this->layers[layer].texCoordsBuffer);
		Main::getBlocksShader().vertexesLocation->setVertexBuffer(*this->layers[layer].vertexesBuffer);
		Main::getBlocksShader().colorsLocation->setVertexBuffer(*this->layers[layer].colorsBuffer);
		this->layers[layer].indicesBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, this->layers[layer].verticesNb, GL_UNSIGNED_INT, NULL);
	}

	void Chunk::setBlockLightRec(glm::vec3 pos, uint8_t light)
	{
		ChunkBlock *block = getBlock(pos);
		if (!block)
			return;
		Block *blockModel = Blocks::getBlock(block->getType());
		if (!blockModel || blockModel->getOpacity() == 15)
			return;
		bool isOverTopBlock = false;
		if (pos.y >= this->topBlocks[getXZId(pos.x, pos.z)])
		{
			light = std::max(light, uint8_t(0xf));
			isOverTopBlock = false;
			goto endNearTop;
		}
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
			if (!this->chunkXLess)
			{
				light = 0;
				goto endNearTop;
			}
			if (pos.y > this->chunkXLess->getTopBlock(CHUNK_WIDTH - 1, pos.z))
			{
				light = std::max(light, uint8_t(0xf));
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
			if (!this->chunkXMore)
			{
				light = 0;
				goto endNearTop;
			}
			if (pos.y > this->chunkXMore->getTopBlock(0, pos.z))
			{
				light = std::max(light, uint8_t(0xf));
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
			if (!this->chunkZLess)
			{
				light = 0;
				goto endNearTop;
			}
			if (pos.y > this->chunkZLess->getTopBlock(pos.x, CHUNK_WIDTH - 1))
			{
				light = std::max(light, uint8_t(0xf));
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
			if (!this->chunkZMore)
			{
				light = 0;
				goto endNearTop;
			}
			if (pos.y > this->chunkZMore->getTopBlock(pos.x, 0))
			{
				light = std::max(light, uint8_t(0xf));
				goto endNearTop;
			}
		}
endNearTop:
		if (pos.x == 0 && this->chunkXLess)
		{
			uint8_t nearLight = this->chunkXLess->getSkyLight(glm::vec3(CHUNK_WIDTH - 1, pos.y, pos.z));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		else if (pos.x == CHUNK_WIDTH - 1 && this->chunkXMore)
		{
			uint8_t nearLight = this->chunkXMore->getSkyLight(glm::vec3(0, pos.y, pos.z));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		if (pos.z == 0 && this->chunkZLess)
		{
			uint8_t nearLight = this->chunkZLess->getSkyLight(glm::vec3(pos.x, pos.y, CHUNK_WIDTH - 1));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		else if (pos.z == CHUNK_WIDTH - 1 && this->chunkZMore)
		{
			uint8_t nearLight = this->chunkZMore->getSkyLight(glm::vec3(pos.x, pos.y, 0));
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		if (light == 0)
			return;
		if (getSkyLight(pos) >= light)
			return;
		setSkyLight(pos, light);
		regenerateBuffers();
		if (blockModel->getOpacity() > light)
			return;
		light -= blockModel->getOpacity();
		/*if (blockModel->getOpacity() == 15)
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
		}*/
		if (pos.x > 0)
			setBlockLightRec(glm::vec3(pos.x - 1, pos.y, pos.z), light);
		else if (this->chunkXLess && this->chunkXLess->generated && (!isOverTopBlock || pos.y < this->chunkXLess->topBlocks[getXZId(CHUNK_WIDTH - 1, pos.z)]))
			this->chunkXLess->setBlockLightRec(glm::vec3(CHUNK_WIDTH - 1, pos.y, pos.z), light);
		if (pos.x < CHUNK_WIDTH - 1)
			setBlockLightRec(glm::vec3(pos.x + 1, pos.y, pos.z), light);
		else if (this->chunkXMore && this->chunkXMore->generated && (!isOverTopBlock || pos.y < this->chunkXMore->topBlocks[getXZId(0, pos.z)]))
			this->chunkXMore->setBlockLightRec(glm::vec3(0, pos.y, pos.z), light);
		if (pos.y > 0)
			setBlockLightRec(glm::vec3(pos.x, pos.y - 1, pos.z), light);
		if (pos.y < CHUNK_HEIGHT - 1 && pos.y < this->topBlocks[getXZId(pos.x, pos.z)])
			setBlockLightRec(glm::vec3(pos.x, pos.y + 1, pos.z), light);
		if (pos.z > 0)
			setBlockLightRec(glm::vec3(pos.x, pos.y, pos.z - 1), light);
		else if (this->chunkZLess && this->chunkZLess->generated && (!isOverTopBlock || pos.y < this->chunkZLess->topBlocks[getXZId(pos.x, CHUNK_WIDTH - 1)]))
			this->chunkZLess->setBlockLightRec(glm::vec3(pos.x, pos.y, CHUNK_WIDTH - 1), light);
		if (pos.z < CHUNK_WIDTH - 1)
			setBlockLightRec(glm::vec3(pos.x, pos.y, pos.z + 1), light);
		else if (this->chunkZMore && this->chunkZMore->generated && (!isOverTopBlock || pos.y < this->chunkZMore->topBlocks[getXZId(pos.x, 0)]))
			this->chunkZMore->setBlockLightRec(glm::vec3(pos.x, pos.y, 0), light);
	}

	void Chunk::generateLightMap()
	{
		if (this->mustGenerateLightMap)
			this->mustGenerateLightMap = false;
		for (uint8_t i = 0; i < 16; ++i)
		{
			if (!this->storages[i])
				continue;
			this->storages[i]->resetLights();
		}
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				uint8_t topBlock = this->topBlocks[getXZId(x, z)];
				if (topBlock == CHUNK_HEIGHT)
					setBlockLightRec(glm::vec3(x, topBlock, z), 0xf);
				else
					setBlockLightRec(glm::vec3(x, topBlock + 1, z), 0xf);
			}
		}
	}

	void Chunk::generateBuffers()
	{
		this->mustGenerateBuffers = false;
		for (uint8_t layer = 0; layer < 3; ++layer)
		{
			for (uint8_t i = 0; i < 16; ++i)
			{
				if (!this->storages[i])
					continue;
				this->storages[i]->fillBuffers(this, this->layers[layer].tessellator, layer);
			}
		}
		this->mustUpdateBuffers = true;
	}

	void Chunk::updateGLBuffers()
	{
		this->mustUpdateBuffers = false;
		for (uint8_t layer = 0; layer < 3; ++layer)
		{
			if (!this->layers[layer].texCoordsBuffer)
				this->layers[layer].texCoordsBuffer = new VertexBuffer();
			if (!this->layers[layer].vertexesBuffer)
				this->layers[layer].vertexesBuffer = new VertexBuffer();
			if (!this->layers[layer].indicesBuffer)
				this->layers[layer].indicesBuffer = new VertexBuffer();
			if (!this->layers[layer].colorsBuffer)
				this->layers[layer].colorsBuffer = new VertexBuffer();
			this->layers[layer].texCoordsBuffer->setData(GL_ARRAY_BUFFER, this->layers[layer].tessellator.texCoords.data(), this->layers[layer].tessellator.texCoords.size() * sizeof(glm::vec2), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
			this->layers[layer].vertexesBuffer->setData(GL_ARRAY_BUFFER, this->layers[layer].tessellator.vertexes.data(), this->layers[layer].tessellator.vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
			this->layers[layer].indicesBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, this->layers[layer].tessellator.indices.data(), this->layers[layer].tessellator.indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
			this->layers[layer].colorsBuffer->setData(GL_ARRAY_BUFFER, this->layers[layer].tessellator.colors.data(), this->layers[layer].tessellator.colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
			this->layers[layer].verticesNb = this->layers[layer].tessellator.indices.size();
			std::vector<glm::vec2> emptyTexCoords;
			std::vector<glm::vec3> emptyVertexes;
			std::vector<glm::vec3> emptyColors;
			std::vector<GLuint> emptyIndices;
			this->layers[layer].tessellator.texCoords.swap(emptyTexCoords);
			this->layers[layer].tessellator.vertexes.swap(emptyVertexes);
			this->layers[layer].tessellator.colors.swap(emptyColors);
			this->layers[layer].tessellator.indices.swap(emptyIndices);
		}
	}

	void Chunk::setBlockIfReplaceable(glm::vec3 pos, uint8_t type)
	{
		ChunkBlock *block = getBlock(pos);
		if (block)
		{
			Block *blockModel = Blocks::getBlock(block->getType());
			if (blockModel && !blockModel->isReplaceable())
				return;
		}
		setBlock(pos, type);
	}

	void Chunk::setBlock(glm::vec3 pos, uint8_t type)
	{
		if (type)
		{
			uint8_t *top = &this->topBlocks[getXZId(pos.x, pos.z)];
			if (pos.y > *top)
				*top = pos.y;
		}
		int32_t storageY = pos.y / 16;
		if (!this->storages[storageY])
			this->storages[storageY] = new ChunkStorage(storageY * 16);
		this->storages[storageY]->setBlock(glm::vec3(pos.x, pos.y - storageY * 16, pos.z), type);
		regenerateLightMap();
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
	}

	ChunkBlock *Chunk::getBlock(glm::vec3 pos)
	{
		uint8_t storageY = pos.y / 16;
		if (!this->storages[storageY])
			return (NULL);
		return (this->storages[storageY]->getBlock(glm::vec3(pos.x, pos.y - storageY * 16, pos.z)));
	}

	uint8_t Chunk::getLight(glm::vec3 pos)
	{
		if (!this->generated)
			return (0);
		uint8_t storageY = pos.y / 16;
		if (!this->storages[storageY])
			return (0);
		return (this->storages[storageY]->getLight(glm::vec3(pos.x, pos.y - storageY * 16, pos.z)));
	}

	void Chunk::setSkyLight(glm::vec3 pos, uint8_t light)
	{
		uint8_t storageY = pos.y / 16;
		if (!this->storages[storageY])
			return;
		return (this->storages[storageY]->setSkyLight(glm::vec3(pos.x, pos.y - storageY * 16, pos.z), light));
	}

	uint8_t Chunk::getSkyLight(glm::vec3 pos)
	{
		if (!this->generated)
			return (0);
		uint8_t storageY = pos.y / 16;
		if (!this->storages[storageY])
			return (0);
		return (this->storages[storageY]->getSkyLight(glm::vec3(pos.x, pos.y - storageY * 16, pos.z)));
	}

	void Chunk::setBlockLight(glm::vec3 pos, uint8_t light)
	{
		uint8_t storageY = pos.y / 16;
		if (!this->storages[storageY])
			return;
		return (this->storages[storageY]->setSkyLight(glm::vec3(pos.x, pos.y - storageY * 16, pos.z), light));
	}

	uint8_t Chunk::getBlockLight(glm::vec3 pos)
	{
		if (!this->generated)
			return (0);
		uint8_t storageY = pos.y / 16;
		if (!this->storages[storageY])
			return (0);
		return (this->storages[storageY]->getBlockLight(glm::vec3(pos.x, pos.y - storageY * 16, pos.z)));
	}

	uint8_t Chunk::getTopBlock(int32_t x, int32_t z)
	{
		if (!this->generated)
			return (CHUNK_HEIGHT);
		return (this->topBlocks[getXZId(x, z)]);
	}

	void Chunk::destroyBlock(glm::vec3 pos)
	{
		uint8_t *top = &this->topBlocks[getXZId(pos.x, pos.z)];
		if ((uint8_t)pos.y == *top)
		{
			for (int16_t i = *top - 1; i >= 0; --i)
			{
				ChunkBlock *block = getBlock(glm::vec3(pos.x, i, pos.z));
				if (block && block->getType())
				{
					*top = (uint8_t)i;
					break;
				}
			}
		}
		setBlock(pos, 0);
		regenerateLightMap();
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
	}

	void Chunk::regenerateBuffers()
	{
		if (!this->generated)
			return;
		if (this->mustGenerateBuffers)
			return;
		this->mustGenerateBuffers = true;
		this->world.getChunksToUpdate().push_back(this);
	}

	void Chunk::regenerateLightMap()
	{
		if (!this->generated)
			return;
		if (this->mustGenerateLightMap)
			return;
		this->mustGenerateBuffers = true;
		this->mustGenerateLightMap = true;
		this->world.getChunksToUpdate().push_back(this);
	}

	void Chunk::setChunkXLess(Chunk *chunk)
	{
		this->chunkXLess = chunk;
	}

	void Chunk::setChunkXMore(Chunk *chunk)
	{
		this->chunkXMore = chunk;
	}

	void Chunk::setChunkZLess(Chunk *chunk)
	{
		this->chunkZLess = chunk;
	}

	void Chunk::setChunkZMore(Chunk *chunk)
	{
		this->chunkZMore = chunk;
	}

}
