#include "Chunk.h"
#include "World/Biomes/Biomes.h"
#include "Noise/WorleyNoise.h"
#include "ChunkTessellator.h"
#include "Blocks/Blocks.h"
#include "Utils/System.h"
#include "NBT/NBT.h"
#include "Debug.h"
#include "World.h"
#include "Main.h"
#include <cstring>

extern int64_t nanotime;

namespace voxel
{

	Chunk::Chunk(World &world, int32_t x, int32_t z)
	: particlesManager(*this)
	, entitiesManager(*this)
	, world(world)
	, aabb(Vec3(x, 0, z), Vec3(x + CHUNK_WIDTH, CHUNK_HEIGHT, z + CHUNK_WIDTH))
	, x(x)
	, z(z)
	, mustGenerateLightMap(false)
	, mustGenerateBuffers(false)
	, mustUpdateBuffers(false)
	, recursiveLightMap(false)
	, deleted(false)
	, changed(false)
	{
		if ((this->chunkXLess = this->world.getChunk(this->x - CHUNK_WIDTH, this->z)))
			this->chunkXLess->setChunkXMore(this);
		if ((this->chunkXMore = this->world.getChunk(this->x + CHUNK_WIDTH, this->z)))
			this->chunkXMore->setChunkXLess(this);
		if ((this->chunkZLess = this->world.getChunk(this->x, this->z - CHUNK_WIDTH)))
			this->chunkZLess->setChunkZMore(this);
		if ((this->chunkZMore = this->world.getChunk(this->x, this->z + CHUNK_WIDTH)))
			this->chunkZMore->setChunkZLess(this);
		std::memset(this->storages, 0, sizeof(this->storages));
	}

	Chunk::~Chunk()
	{
		for (uint8_t i = 0; i < 16; ++i)
			delete (this->storages[i]);
		delete (this->NBT.NBT);
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
				this->world.getVBOToDelete().push_back(layer.texCoordsBuffer);
			if (layer.vertexesBuffer)
				this->world.getVBOToDelete().push_back(layer.vertexesBuffer);
			if (layer.indicesBuffer)
				this->world.getVBOToDelete().push_back(layer.indicesBuffer);
			if (layer.colorsBuffer)
				this->world.getVBOToDelete().push_back(layer.colorsBuffer);
			if (layer.vertexArray)
				this->world.getVAOToDelete().push_back(layer.vertexArray);
		}
	}

	void Chunk::generate()
	{
		if (isGenerated())
			return;
		setGenerated(true);
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				//float fac = 1. / 50;
				//float temp = this->world.getBiomeTempNoise().get2((this->x + x) * fac, (this->z + z) * fac) / 2 + .5;
				//float rain = this->world.getBiomeRainNoise().get2((this->x + x) * fac, (this->z + z) * fac) / 2 + .5;
				//float temp = std::min(1., std::max(-1., WorleyNoise::get2((this->x + x) * fac, (this->z + z) * fac)));
				//float rain = 1.;//std::min(1., std::max(-1., WorleyNoise::get2((this->x + x) * fac / 100, (this->z + z) * fac / 100)));
				//this->biomes[getXZId(x, z)] = Biomes::getBiomeFor(temp, rain);
				//float noiseIndex = -.02;
				//float noiseIndex = 0;
				float noiseIndex = this->world.getNoise().get2((this->x + x) * 100, (this->z + z) * 100);
				//float noiseIndex = std::min(1., std::max(-1., WorleyNoise::get2((this->x + x) / 50., (this->z + z) / 50.)));
				//noiseIndex *= this->world.getNoise().get2(this->x + x, this->z + z);
				//float noiseIndex = this->world.getNoise().get2(this->x + x, this->z + z) / 2;
				//noiseIndex += this->world.getNoise().get3(this->x + x, this->z + z, 3) / 3;
				//noiseIndex += this->world.getNoise().get3(this->x + x, this->z + z, 300000) / 4;
				noiseIndex = noiseIndex * CHUNK_HEIGHT / 5 + CHUNK_HEIGHT / 4;
				noiseIndex = std::round(noiseIndex);
				if ((x + z) % 2 == 0)
					noiseIndex--;
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
							blockType = rand() / (float)RAND_MAX * 255;//2;
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
					setBlockIfReplaceable(x, y, z, blockType);
				}
			}
		}
		Biomes::getBiome(1)->generate(*this);
		regenerateLightMapRec();
	}

	void Chunk::tick()
	{
		this->entitiesManager.tick();
		this->particlesManager.tick();
	}

	void Chunk::drawEntities()
	{
		if (!isGenerated() || !this->visible)
			return;
		this->particlesManager.draw();
		this->entitiesManager.draw();
	}

	void Chunk::draw(uint8_t layer)
	{
		if (!isGenerated())
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
		if (layer == 3)
			updateGLBuffer(3);
		Main::getBlocksShader().texCoordsLocation->setVertexBuffer(*this->layers[layer].texCoordsBuffer);
		Main::getBlocksShader().vertexesLocation->setVertexBuffer(*this->layers[layer].vertexesBuffer);
		Main::getBlocksShader().colorsLocation->setVertexBuffer(*this->layers[layer].colorsBuffer);
		this->layers[layer].indicesBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, this->layers[layer].verticesNb, GL_UNSIGNED_INT, NULL);
	}

	void Chunk::setBlockLightRec(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		ChunkBlock *block = getBlock(x, y, z);
		uint8_t type = !block ? 0 : block->getType();
		Block *blockModel = Blocks::getBlock(type);
		if (!blockModel)
			blockModel = Blocks::getBlock(0);
		if (blockModel->getOpacity() == 15)
			return;
		bool isTopBlock = false;
		if (light == 0xf)
			goto endNearTop;
		if (y >= getTopBlock(x, z))
		{
			isTopBlock = true;
			light = 0xf;
			goto endNearTop;
		}
		for (int8_t xx = -1; xx <= 1; ++xx)
		{
			for (int8_t yy = -1; yy <= 1; ++yy)
			{
				for (int8_t zz = -1; zz <= 1; ++zz)
				{
					ChunkBlock *tmp = this->world.getBlock(this->x + xx + x, yy + y, this->z + zz + z);
					if (tmp && tmp->getType())
						goto endCheckAround;
				}
			}
		}
		return;
	endCheckAround:
		if (x > 0)
		{
			if (y > getTopBlock(x - 1, z))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkXLess && y > this->chunkXLess->getTopBlock(CHUNK_WIDTH - 1, z))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		if (x < CHUNK_WIDTH - 1)
		{
			if (y > getTopBlock(x + 1, z))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkXMore && y > this->chunkXMore->getTopBlock(0, z))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		if (z > 0)
		{
			if (y > getTopBlock(x, z - 1))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkZLess && y > this->chunkZLess->getTopBlock(x, CHUNK_WIDTH - 1))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		if (z < CHUNK_WIDTH - 1)
		{
			if (y > getTopBlock(x, z + 1))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		else
		{
			if (this->chunkZMore && y > this->chunkZMore->getTopBlock(x, 0))
			{
				light = 0xf;
				goto endNearTop;
			}
		}
		if (x == 0 && this->chunkXLess)
		{
			uint8_t nearLight = this->chunkXLess->getSkyLight(CHUNK_WIDTH - 1, y, z);
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		else if (x == CHUNK_WIDTH - 1 && this->chunkXMore)
		{
			uint8_t nearLight = this->chunkXMore->getSkyLight(0, y, z);
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		if (z == 0 && this->chunkZLess)
		{
			uint8_t nearLight = this->chunkZLess->getSkyLight(x, y, CHUNK_WIDTH - 1);
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
		else if (z == CHUNK_WIDTH - 1 && this->chunkZMore)
		{
			uint8_t nearLight = this->chunkZMore->getSkyLight(x, y, 0);
			if (nearLight > 0)
				light = std::max(uint8_t(nearLight - 1u), light);
		}
endNearTop:
		if (getSkyLightVal(x, y, z) >= light)
			return;
		setSkyLight(x, y, z, light);
		if (blockModel->getOpacity() >= light)
			return;
		light -= blockModel->getOpacity();
		if (x > 0)// && !isTopBlock && pos.y < this->topBlocks[getXZId(pos.x - 1, pos.z)])
			setBlockLightRec(x - 1, y, z, light);
		if (x < CHUNK_WIDTH - 1)// && !isTopBlock && pos.y < this->topBlocks[getXZId(pos.x + 1, pos.z)])
			setBlockLightRec(x + 1, y, z, light);
		if (y > 0)
			setBlockLightRec(x, y - 1, z, light);
		if (y < CHUNK_HEIGHT - 1 && !isTopBlock)
			setBlockLightRec(x, y + 1, z, light);
		if (z > 0)// && !isTopBlock && pos.y < this->topBlocks[getXZId(pos.x, pos.z - 1)])
			setBlockLightRec(x, y, z - 1, light);
		if (z < CHUNK_WIDTH - 1)// && !isTopBlock && pos.y < this->topBlocks[getXZId(pos.x, pos.z + 1)])
			setBlockLightRec(x, y, z + 1, light);
	}

	void Chunk::generateLightMap()
	{
		this->mustGenerateLightMap = false;
		if (this->recursiveLightMap)
		{
			this->recursiveLightMap = false;
			if (this->chunkXLess)
				this->chunkXLess->regenerateLightMap();
			if (this->chunkXMore)
				this->chunkXMore->regenerateLightMap();
			if (this->chunkZLess)
				this->chunkZLess->regenerateLightMap();
			if (this->chunkZMore)
				this->chunkZMore->regenerateLightMap();
		}
		for (uint8_t i = 0; i < 16; ++i)
		{
			ChunkStorage *storage = getStorage(i);
			if (!storage)
				continue;
			storage->resetLights();
		}
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				uint8_t topBlock = getTopBlock(x, z);
				if (topBlock == CHUNK_HEIGHT)
					setBlockLightRec(x, topBlock, z, 0xf);
				else
					setBlockLightRec(x, topBlock + 1, z, 0xe);
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
				ChunkStorage *storage = getStorage(i);
				if (!storage)
					continue;
				storage->fillBuffers(this, this->layers[layer].tessellator, layer);
			}
		}
		this->mustUpdateBuffers = true;
	}

	void Chunk::updateGLBuffer(uint8_t layer)
	{
		Main::getBlocksShader().program->use();
		if (!this->layers[layer].texCoordsBuffer)
			this->layers[layer].texCoordsBuffer = new VertexBuffer();
		if (!this->layers[layer].vertexesBuffer)
			this->layers[layer].vertexesBuffer = new VertexBuffer();
		if (!this->layers[layer].indicesBuffer)
			this->layers[layer].indicesBuffer = new VertexBuffer();
		if (!this->layers[layer].colorsBuffer)
			this->layers[layer].colorsBuffer = new VertexBuffer();
		this->layers[layer].texCoordsBuffer->setData(GL_ARRAY_BUFFER, this->layers[layer].tessellator.texCoords.data(), this->layers[layer].tessellator.texCoords.size() * sizeof(Vec2), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->layers[layer].vertexesBuffer->setData(GL_ARRAY_BUFFER, this->layers[layer].tessellator.vertexes.data(), this->layers[layer].tessellator.vertexes.size() * sizeof(Vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->layers[layer].indicesBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, this->layers[layer].tessellator.indices.data(), this->layers[layer].tessellator.indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
		this->layers[layer].colorsBuffer->setData(GL_ARRAY_BUFFER, this->layers[layer].tessellator.colors.data(), this->layers[layer].tessellator.colors.size() * sizeof(Vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->layers[layer].verticesNb = this->layers[layer].tessellator.indices.size();
		std::vector<Vec2> emptyTexCoords;
		std::vector<Vec3> emptyVertexes;
		std::vector<Vec3> emptyColors;
		std::vector<GLuint> emptyIndices;
		this->layers[layer].tessellator.texCoords.swap(emptyTexCoords);
		this->layers[layer].tessellator.vertexes.swap(emptyVertexes);
		this->layers[layer].tessellator.colors.swap(emptyColors);
		this->layers[layer].tessellator.indices.swap(emptyIndices);
	}

	void Chunk::updateGLBuffers()
	{
		this->mustUpdateBuffers = false;
		for (uint8_t layer = 0; layer < 3; ++layer)
			updateGLBuffer(layer);
	}

	void Chunk::setBlockIfReplaceable(int32_t x, int32_t y, int32_t z, uint8_t type)
	{
		ChunkBlock *block = getBlock(x, y, z);
		if (block)
		{
			Block *blockModel = Blocks::getBlock(block->getType());
			if (blockModel && !blockModel->isReplaceable())
				return;
		}
		setBlock(x, y, z, type);
	}

	void Chunk::setBlock(int32_t x, int32_t y, int32_t z, uint8_t type)
	{
		if (type)
		{
			uint8_t top = getTopBlock(x, z);
			if (y > top)
				setTopBlock(x, z, y);
		}
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
		{
			if (!type)
				return;
			storage = createStorage(y / 16);
		}
		storage->setBlock(x, y - storage->getId() * 16, z, type);
		this->changed = true;
		regenerateLightMap();
		if (x == 0)
		{
			if (this->chunkXLess)
				this->chunkXLess->regenerateLightMap();
		}
		else if (x == CHUNK_WIDTH - 1)
		{
			if (this->chunkXMore)
				this->chunkXMore->regenerateLightMap();
		}
		if (z == 0)
		{
			if (this->chunkZLess)
				this->chunkZLess->regenerateLightMap();
		}
		else if (z == CHUNK_WIDTH - 1)
		{
			if (this->chunkZMore)
				this->chunkZMore->regenerateLightMap();
		}
	}

	ChunkBlock *Chunk::getBlock(int32_t x, int32_t y, int32_t z)
	{
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
			return (NULL);
		return (storage->getBlock(x, y - storage->getId() * 16, z));
	}

	uint8_t Chunk::getLight(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return (0);
		if (y > getTopBlock(x, z))
			return (15);
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
			return (15);
		return (storage->getLight(x, y - storage->getId() * 16, z));
	}

	void Chunk::setSkyLight(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
			storage = createStorage(y / 16);
		storage->setSkyLight(x, y - storage->getId() * 16, z, light);
		this->changed = true;
	}

	uint8_t Chunk::getSkyLightVal(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return (0);
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
			return (0);
		return (storage->getSkyLight(x, y - storage->getId() * 16, z));
	}

	uint8_t Chunk::getSkyLight(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return (0);
		if (y > getTopBlock(x, z))
			return (15);
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
			return (15);
		return (storage->getSkyLight(x, y - storage->getId() * 16, z));
	}

	void Chunk::setBlockLight(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
			return;
		storage->setSkyLight(x, y - storage->getId() * 16, z, light);
		this->changed = true;
	}

	uint8_t Chunk::getBlockLight(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return (0);
		ChunkStorage *storage = getStorage(y / 16);
		if (!storage)
			return (0);
		return (storage->getBlockLight(x, y - storage->getId() * 16, z));
	}

	void Chunk::setTopBlock(int32_t x, int32_t z, uint8_t top)
	{
		this->NBT.HeightMap->getValues()[getXZId(x, z)] = top;
	}

	uint8_t Chunk::getTopBlock(int32_t x, int32_t z)
	{
		if (!isGenerated())
			return (CHUNK_HEIGHT);
		return (this->NBT.HeightMap->getValues()[getXZId(x, z)]);
	}

	void Chunk::setBiome(int32_t x, int32_t z, uint8_t biome)
	{
		this->NBT.Biomes->getValues()[getXZId(x, z)] = biome;
	}

	uint8_t Chunk::getBiome(int32_t x, int32_t z)
	{
		return (this->NBT.Biomes->getValues()[getXZId(x, z)]);
	}

	void Chunk::destroyBlock(int32_t x, int32_t y, int32_t z)
	{
		uint8_t top = getTopBlock(x, z);
		if (y == top)
		{
			for (int16_t i = top - 1; i >= 0; --i)
			{
				ChunkBlock *block = getBlock(x, i, z);
				if (block && block->getType())
				{
					setTopBlock(x, z, i);
					break;
				}
			}
		}
		setBlock(x, y, z, 0);
		regenerateLightMap();
		if (x == 0)
		{
			if (this->chunkXLess)
				this->chunkXLess->regenerateLightMap();
		}
		else if (x == CHUNK_WIDTH - 1)
		{
			if (this->chunkXMore)
				this->chunkXMore->regenerateLightMap();
		}
		if (z == 0)
		{
			if (this->chunkZLess)
				this->chunkZLess->regenerateLightMap();
		}
		else if (z == CHUNK_WIDTH - 1)
		{
			if (this->chunkZMore)
				this->chunkZMore->regenerateLightMap();
		}
	}

	void Chunk::regenerateBuffers()
	{
		if (!isGenerated())
			return;
		if (this->mustGenerateBuffers)
			return;
		this->mustGenerateBuffers = true;
		this->world.getChunksToUpdate().push_back(this);
	}

	void Chunk::regenerateLightMap()
	{
		if (!isGenerated())
			return;
		if (this->mustGenerateLightMap)
			return;
		bool tmp = this->mustGenerateBuffers;
		this->mustGenerateBuffers = true;
		this->mustGenerateLightMap = true;
		if (!tmp) //Already in list
			this->world.getChunksToUpdate().push_back(this);
	}

	void Chunk::regenerateLightMapRec()
	{
		//this->recursiveLightMap = true;
		regenerateLightMap();
	}

	ChunkStorage *Chunk::getStorage(uint8_t id)
	{
		return (this->storages[id]);
	}

	ChunkStorage *Chunk::createStorage(uint8_t id)
	{
		if (this->storages[id])
			return (this->storages[id]);
		NBTTagCompound *NBT = NULL;
		this->storages[id] = new ChunkStorage(id);
		for (uint32_t i = 0; i < this->NBT.Sections->getValues().size(); ++i)
		{
			NBTTagCompound *section = reinterpret_cast<NBTTagCompound*>(this->NBT.Sections->getValues()[i]);
			for (uint32_t i = 0; i < section->getTags().size(); ++i)
			{
				NBTTagByte *Y = reinterpret_cast<NBTTagByte*>(section->getTags()[i]);
				if (Y->getValue() != id)
					continue;
				NBT = section;
				goto found;
			}
		}
		NBT = new NBTTagCompound("");
		this->NBT.Sections->addValue(NBT);
found:
		this->storages[id]->initNBT(NBT);
		return (this->storages[id]);
	}

	void Chunk::initNBT(NBTTagCompound *NBT)
	{
		std::memset(&this->NBT, 0, sizeof(this->NBT));
		this->NBT.NBT = NBT;
		if (!this->NBT.NBT)
			this->NBT.NBT = new NBTTagCompound("");
		for (std::vector<NBTTag*>::iterator iter = this->NBT.NBT->getTags().begin(); iter != this->NBT.NBT->getTags().end(); ++iter)
		{
			if (!(*iter)->getName().compare("DataVersion") && (*iter)->getType() == NBT_TAG_INT)
				this->NBT.DataVersion = reinterpret_cast<NBTTagInt*>(*iter);
			else if (!(*iter)->getName().compare("Level") && (*iter)->getType() == NBT_TAG_COMPOUND)
				this->NBT.Level = reinterpret_cast<NBTTagCompound*>(*iter);
			else
				goto erase1;
			continue;
erase1:
			this->NBT.NBT->getTags().erase(iter);
			iter = this->NBT.NBT->getTags().begin();
			if (iter == this->NBT.NBT->getTags().end())
				break;
		}
		if (!this->NBT.DataVersion)
		{
			this->NBT.DataVersion = new NBTTagInt("DataVersion");
			this->NBT.DataVersion->setValue(1);
			this->NBT.NBT->addTag(this->NBT.DataVersion);
		}
		if (!this->NBT.Level)
		{
			this->NBT.Level = new NBTTagCompound("Level");
			this->NBT.NBT->addTag(this->NBT.Level);
		}
		for (std::vector<NBTTag*>::iterator iter = this->NBT.Level->getTags().begin(); iter != this->NBT.Level->getTags().end(); ++iter)
		{
			if (!(*iter)->getName().compare("xPos") && (*iter)->getType() == NBT_TAG_INT)
				this->NBT.xPos = reinterpret_cast<NBTTagInt*>(*iter);
			else if (!(*iter)->getName().compare("zPos") && (*iter)->getType() == NBT_TAG_INT)
				this->NBT.zPos = reinterpret_cast<NBTTagInt*>(*iter);
			else if (!(*iter)->getName().compare("LastUpdate") && (*iter)->getType() == NBT_TAG_LONG)
				this->NBT.LastUpdate = reinterpret_cast<NBTTagLong*>(*iter);
			else if (!(*iter)->getName().compare("LightPopulated") && (*iter)->getType() == NBT_TAG_BYTE)
				this->NBT.LightPopulated = reinterpret_cast<NBTTagByte*>(*iter);
			else if (!(*iter)->getName().compare("TerrainPopulated") && (*iter)->getType() == NBT_TAG_BYTE)
				this->NBT.TerrainPopulated = reinterpret_cast<NBTTagByte*>(*iter);
			else if (!(*iter)->getName().compare("V") && (*iter)->getType() == NBT_TAG_BYTE)
				this->NBT.V = reinterpret_cast<NBTTagByte*>(*iter);
			else if (!(*iter)->getName().compare("InhabitedTime") && (*iter)->getType() == NBT_TAG_LONG)
				this->NBT.InhabitedTime = reinterpret_cast<NBTTagLong*>(*iter);
			else if (!(*iter)->getName().compare("Biomes") && (*iter)->getType() == NBT_TAG_BYTE_ARRAY)
				this->NBT.Biomes = reinterpret_cast<NBTTagByteArray*>(*iter);
			else if (!(*iter)->getName().compare("HeightMap") && (*iter)->getType() == NBT_TAG_BYTE_ARRAY)
				this->NBT.HeightMap = reinterpret_cast<NBTTagByteArray*>(*iter);
			else if (!(*iter)->getName().compare("Sections") && (*iter)->getType() == NBT_TAG_LIST)
				this->NBT.Sections = reinterpret_cast<NBTTagList*>(*iter);
			else if (!(*iter)->getName().compare("Entities") && (*iter)->getType() == NBT_TAG_LIST)
				this->NBT.Entities = reinterpret_cast<NBTTagList*>(*iter);
			else if (!(*iter)->getName().compare("TileEntities") && (*iter)->getType() == NBT_TAG_LIST)
				this->NBT.TileEntities = reinterpret_cast<NBTTagList*>(*iter);
			else if (!(*iter)->getName().compare("TileTicks") && (*iter)->getType() == NBT_TAG_LIST)
				this->NBT.TileTicks = reinterpret_cast<NBTTagList*>(*iter);
			else
				goto erase2;
			continue;
erase2:
			this->NBT.Level->getTags().erase(iter);
			iter = this->NBT.Level->getTags().begin();
			if (iter == this->NBT.Level->getTags().end())
				break;
		}
		if (!this->NBT.xPos)
		{
			this->NBT.xPos = new NBTTagInt("xPos");
			this->NBT.xPos->setValue(this->x);
			this->NBT.Level->addTag(this->NBT.xPos);
		}
		if (!this->NBT.zPos)
		{
			this->NBT.zPos = new NBTTagInt("zPos");
			this->NBT.zPos->setValue(this->z);
			this->NBT.Level->addTag(this->NBT.zPos);
		}
		if (!this->NBT.LastUpdate)
		{
			this->NBT.LastUpdate = new NBTTagLong("LastUpdate");
			this->NBT.LastUpdate->setValue(0);
			this->NBT.Level->addTag(this->NBT.LastUpdate);
		}
		if (!this->NBT.LightPopulated)
		{
			this->NBT.LightPopulated = new NBTTagByte("LightPopulated");
			this->NBT.LightPopulated->setValue(0);
			this->NBT.Level->addTag(this->NBT.LightPopulated);
		}
		if (!this->NBT.TerrainPopulated)
		{
			this->NBT.TerrainPopulated = new NBTTagByte("TerrainPopulated");
			this->NBT.TerrainPopulated->setValue(0);
			this->NBT.Level->addTag(this->NBT.TerrainPopulated);
		}
		if (!this->NBT.V)
		{
			this->NBT.V = new NBTTagByte("V");
			this->NBT.V->setValue(1);
			this->NBT.Level->addTag(this->NBT.V);
		}
		if (!this->NBT.InhabitedTime)
		{
			this->NBT.InhabitedTime = new NBTTagLong("InhabitedTime");
			this->NBT.InhabitedTime->setValue(0);
			this->NBT.Level->addTag(this->NBT.InhabitedTime);
		}
		if (!this->NBT.Biomes)
		{
			this->NBT.Biomes = new NBTTagByteArray("Biomes");
			this->NBT.Biomes->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH, 0);
			this->NBT.Level->addTag(this->NBT.Biomes);
		}
		if (this->NBT.Biomes->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH)
		{
			this->NBT.Biomes->getValues().clear();
			this->NBT.Biomes->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH, 0);
		}
		if (!this->NBT.HeightMap)
		{
			this->NBT.HeightMap = new NBTTagByteArray("HeightMap");
			this->NBT.HeightMap->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH, 0);
			this->NBT.Level->addTag(this->NBT.HeightMap);
		}
		if (this->NBT.HeightMap->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH)
		{
			this->NBT.HeightMap->getValues().clear();
			this->NBT.HeightMap->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH, 0);
		}
		if (!this->NBT.Sections)
		{
			this->NBT.Sections = new NBTTagList("Sections");
			this->NBT.Sections->setType(NBT_TAG_COMPOUND);
			this->NBT.Level->addTag(this->NBT.Sections);
		}
		uint8_t sections[16];
		std::memset(sections, 0, sizeof(sections));
		for (uint32_t i = 0; i < this->NBT.Sections->getValues().size(); ++i)
		{
			NBTTag *tmp = this->NBT.Sections->getValues()[i];
			bool found = false;
			NBTTagCompound *section = reinterpret_cast<NBTTagCompound*>(tmp);
			if (tmp->getType() != NBT_TAG_COMPOUND)
				goto erase3;
			for (uint32_t i = 0; i < section->getTags().size(); ++i)
			{
				if (section->getTags()[i]->getName().compare("Y") || section->getTags()[i]->getType() != NBT_TAG_BYTE)
					continue;
				NBTTagByte *Y = reinterpret_cast<NBTTagByte*>(section->getTags()[i]);
				if (Y->getValue() >= 0 && Y->getValue() <= 15 && !sections[Y->getValue()])
				{
					sections[Y->getValue()] = 1;
					found = true;
				}
				break;
			}
			if (!found)
				goto erase3;
			continue;
erase3:
			delete (tmp);
			this->NBT.Sections->getValues().erase(this->NBT.Sections->getValues().begin() + i);
			i--;
		}
		if (!this->NBT.Entities)
		{
			this->NBT.Entities = new NBTTagList("Entities");
			this->NBT.Entities->setType(NBT_TAG_COMPOUND);
			this->NBT.Level->addTag(this->NBT.Entities);
		}
		if (!this->NBT.TileEntities)
		{
			this->NBT.TileEntities = new NBTTagList("TileEntities");
			this->NBT.TileEntities->setType(NBT_TAG_COMPOUND);
			this->NBT.Level->addTag(this->NBT.TileEntities);
		}
		if (!this->NBT.TileTicks)
		{
			this->NBT.TileTicks = new NBTTagList("TileTicks");
			this->NBT.TileTicks->setType(NBT_TAG_COMPOUND);
			this->NBT.Level->addTag(this->NBT.TileTicks);
		}
	}

	void Chunk::setGenerated(bool generated)
	{
		this->NBT.TerrainPopulated->setValue(generated ? 1 : 0);
	}

	bool Chunk::isGenerated()
	{
		return (this->NBT.TerrainPopulated->getValue());
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
