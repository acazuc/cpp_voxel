#include "Chunk.h"
#include "NBT/NBTCompoundSanitizer.h"
#include "World/Biomes/Biomes.h"
#include "Noise/WorleyNoise.h"
#include "ChunkTessellator.h"
#include "Blocks/Blocks.h"
#include "Utils/System.h"
#include "NBT/NBTMgr.h"
#include "NBT/NBT.h"
#include "World.h"
#include "Debug.h"
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
	, recursiveLightMap(0)
	, x(x)
	, z(z)
	, mustGenerateLightMap(false)
	, mustGenerateBuffers(false)
	, mustUpdateBuffers(false)
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
		std::fill(std::begin(this->storages), std::end(this->storages), nullptr);
	}

	Chunk::~Chunk()
	{
		for (size_t i = 0; i < this->storages.size(); ++i)
			delete (this->storages[i]);
		delete (this->NBT.NBT);
		if (this->chunkXLess)
		{
			this->chunkXLess->setChunkXMore(nullptr);
			this->chunkXLess->regenerateLightMap();
		}
		if (this->chunkXMore)
		{
			this->chunkXMore->setChunkXLess(nullptr);
			this->chunkXMore->regenerateLightMap();
		}
		if (this->chunkZLess)
		{
			this->chunkZLess->setChunkZMore(nullptr);
			this->chunkZLess->regenerateLightMap();
		}
		if (this->chunkZMore)
		{
			this->chunkZMore->setChunkZLess(nullptr);
			this->chunkZMore->regenerateLightMap();
		}
	}

	void Chunk::moveGLBuffersToWorld()
	{
		for (size_t i = 0; i < this->layers.size(); ++i)
		{
			ChunkLayer &layer = this->layers[i];
			if (layer.positionBuffer)
				this->world.getVBOToDelete().push_back(layer.positionBuffer);
			if (layer.indiceBuffer)
				this->world.getVBOToDelete().push_back(layer.indiceBuffer);
			if (layer.colorBuffer)
				this->world.getVBOToDelete().push_back(layer.colorBuffer);
			if (layer.uvBuffer)
				this->world.getVBOToDelete().push_back(layer.uvBuffer);
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
				//float fac = 50;
				//float temp = this->world.getBiomeTempNoise().get2((this->x + x) * fac, (this->z + z) * fac) / 2 + .5;
				//float rain = this->world.getBiomeRainNoise().get2((this->x + x) * fac, (this->z + z) * fac) / 2 + .5;
				//float temp = std::min(1., std::max(-1., WorleyNoise::get2((this->x + x) * fac, (this->z + z) * fac))) / 2 + .5;
				//float rain = std::min(1., std::max(-1., WorleyNoise::get2((this->x + x) * fac, (this->z + z) * fac))) / 2 + .5;
				//this->NBT.Biomes->getValues()[getXZId(x, z)] = Biomes::getBiomeFor(temp, rain);
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
				//if ((x + z) % 2 == 0)
				//	noiseIndex--;
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
							blockType = 2;//rand() / (float)RAND_MAX * 255;//2;
					}
					else if (y > noiseIndex)
						blockType = 8;
					else if (y > noiseIndex - 3)
					{
						if (y < CHUNK_HEIGHT / 4)
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
		if (!this->layers[layer].indicesNb)
			return;
		//if (layer == 3)
		//	updateGLBuffer(3);
		this->layers[layer].vertexArray->bind();
		glDrawElements(GL_TRIANGLES, this->layers[layer].indicesNb, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
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
		for (size_t i = 0; i < this->storages.size(); ++i)
		{
			ChunkStorage *storage = this->storages[i];
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
		if (this->recursiveLightMap & 0x1 && this->chunkXLess)
			this->chunkXLess->regenerateLightMap();
		if (this->recursiveLightMap & 0x2 && this->chunkXMore)
			this->chunkXMore->regenerateLightMap();
		if (this->recursiveLightMap & 0x4 && this->chunkZLess)
			this->chunkZLess->regenerateLightMap();
		if (this->recursiveLightMap & 0x8 && this->chunkZMore)
			this->chunkZMore->regenerateLightMap();
		this->recursiveLightMap = 0;
	}

	void Chunk::generateBuffers()
	{
		this->mustGenerateBuffers = false;
		for (uint8_t layer = 0; layer < this->layers.size(); ++layer)
		{
			for (uint8_t i = 0; i < this->storages.size(); ++i)
			{
				ChunkStorage *storage = this->storages[i];
				if (!storage)
					continue;
				storage->fillBuffers(this, this->layers[layer].tessellator, layer);
			}
		}
		this->mustUpdateBuffers = true;
	}

	void Chunk::updateGLBuffer(uint8_t layer)
	{
		if (!this->layers[layer].positionBuffer)
			this->layers[layer].positionBuffer = new VertexBuffer();
		if (!this->layers[layer].indiceBuffer)
			this->layers[layer].indiceBuffer = new VertexBuffer();
		if (!this->layers[layer].colorBuffer)
			this->layers[layer].colorBuffer = new VertexBuffer();
		if (!this->layers[layer].uvBuffer)
			this->layers[layer].uvBuffer = new VertexBuffer();
		if (!this->layers[layer].vertexArray)
		{
			this->layers[layer].vertexArray = new VertexArray();
			this->layers[layer].vertexArray->bind();
			Main::getBlocksShader().vertexPositionLocation.setVertexBuffer(*this->layers[layer].positionBuffer, 3, GL_FLOAT);
			Main::getBlocksShader().vertexPositionLocation.setVertexAttribArray(true);
			Main::getBlocksShader().vertexColorLocation.setVertexBuffer(*this->layers[layer].colorBuffer, 3, GL_FLOAT);
			Main::getBlocksShader().vertexColorLocation.setVertexAttribArray(true);
			Main::getBlocksShader().vertexUVLocation.setVertexBuffer(*this->layers[layer].uvBuffer, 2, GL_FLOAT);
			Main::getBlocksShader().vertexUVLocation.setVertexAttribArray(true);
			this->layers[layer].indiceBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
			glBindVertexArray(0);
		}
		std::vector<Vec3> &positions = this->layers[layer].tessellator.positions;
		std::vector<Vec3> &colors = this->layers[layer].tessellator.colors;
		std::vector<Vec2> &uvs = this->layers[layer].tessellator.uvs;
		std::vector<GLuint> &indices = this->layers[layer].tessellator.indices;
		this->layers[layer].positionBuffer->setData(GL_ARRAY_BUFFER, positions.data(), positions.size() * sizeof(*positions.data()), GL_STATIC_DRAW);
		this->layers[layer].colorBuffer->setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(*colors.data()), GL_STATIC_DRAW);
		this->layers[layer].uvBuffer->setData(GL_ARRAY_BUFFER, uvs.data(), uvs.size() * sizeof(*uvs.data()), GL_STATIC_DRAW);
		this->layers[layer].indiceBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(*indices.data()), GL_STATIC_DRAW);
		this->layers[layer].indicesNb = this->layers[layer].tessellator.indices.size();
		std::vector<Vec2> emptyUVs;
		std::vector<Vec3> emptyColors;
		std::vector<Vec3> emptyPositions;
		std::vector<GLuint> emptyIndices;
		this->layers[layer].tessellator.positions.swap(emptyPositions);
		this->layers[layer].tessellator.colors.swap(emptyColors);
		this->layers[layer].tessellator.uvs.swap(emptyUVs);
		this->layers[layer].tessellator.indices.swap(emptyIndices);
	}

	void Chunk::updateGLBuffers()
	{
		this->mustUpdateBuffers = false;
		for (size_t layer = 0; layer < this->layers.size(); ++layer)
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
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
		{
			if (!type)
				return;
			storage = createStorage(y / CHUNK_STORAGE_HEIGHT);
		}
		storage->setBlock(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z, type);
		this->changed = true;
		regenerateLightMap();
		if (x == 0)
			this->recursiveLightMap |= 0x1;
		else if (x == CHUNK_WIDTH - 1)
			this->recursiveLightMap |= 0x2;
		if (z == 0)
			this->recursiveLightMap |= 0x4;
		else if (z == CHUNK_WIDTH - 1)
			this->recursiveLightMap |= 0x8;
	}

	ChunkBlock *Chunk::getBlock(int32_t x, int32_t y, int32_t z)
	{
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
			return nullptr;
		return storage->getBlock(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z);
	}

	uint8_t Chunk::getLight(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return 0;
		if (y > getTopBlock(x, z))
			return 15;
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
			return 15;
		return storage->getLight(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z);
	}

	void Chunk::setSkyLight(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
			storage = createStorage(y / CHUNK_STORAGE_HEIGHT);
		storage->setSkyLight(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z, light);
		this->changed = true;
	}

	uint8_t Chunk::getSkyLightVal(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return 0;
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
			return 0;
		return storage->getSkyLight(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z);
	}

	uint8_t Chunk::getSkyLight(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return 0;
		if (y > getTopBlock(x, z))
			return 15;
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
			return 15;
		return storage->getSkyLight(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z);
	}

	void Chunk::setBlockLight(int32_t x, int32_t y, int32_t z, uint8_t light)
	{
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
			return;
		storage->setSkyLight(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z, light);
		this->changed = true;
	}

	uint8_t Chunk::getBlockLight(int32_t x, int32_t y, int32_t z)
	{
		if (!isGenerated())
			return 0;
		ChunkStorage *storage = getStorage(y / CHUNK_STORAGE_HEIGHT);
		if (!storage)
			return 0;
		return storage->getBlockLight(x, y - storage->getId() * CHUNK_STORAGE_HEIGHT, z);
	}

	void Chunk::setTopBlock(int32_t x, int32_t z, uint8_t top)
	{
		this->NBT.HeightMap->getValues()[getXZId(x, z)] = top;
	}

	uint8_t Chunk::getTopBlock(int32_t x, int32_t z)
	{
		if (!isGenerated())
			return CHUNK_HEIGHT;
		return this->NBT.HeightMap->getValues()[getXZId(x, z)];
	}

	void Chunk::setBiome(int32_t x, int32_t z, uint8_t biome)
	{
		this->NBT.Biomes->getValues()[getXZId(x, z)] = biome;
	}

	uint8_t Chunk::getBiome(int32_t x, int32_t z)
	{
		return this->NBT.Biomes->getValues()[getXZId(x, z)];
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
		this->mustGenerateLightMap = true;
		regenerateBuffers();
	}

	void Chunk::regenerateLightMapRec()
	{
		this->recursiveLightMap = 0xff;
		regenerateLightMap();
	}

	ChunkStorage *Chunk::getStorage(uint8_t id)
	{
		return this->storages[id];
	}

	ChunkStorage *Chunk::createStorage(uint8_t id)
	{
		if (this->storages[id])
			return this->storages[id];
		NBTTagCompound *NBT = nullptr;
		this->storages[id] = new ChunkStorage(id);
		for (size_t i = 0; i < this->NBT.Sections->getValues().size(); ++i)
		{
			NBTTagCompound *section = reinterpret_cast<NBTTagCompound*>(this->NBT.Sections->getValues()[i]);
			for (size_t i = 0; i < section->getTags().size(); ++i)
			{
				NBTTag *tag = section->getTags()[i];
				if (tag->getName().compare("Y") || tag->getType() != NBT_TAG_BYTE)
					continue;
				NBTTagByte *Y = reinterpret_cast<NBTTagByte*>(tag);
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
		return this->storages[id];
	}

	void Chunk::initNBT(NBTTagCompound *NBT)
	{
		std::memset(&this->NBT, 0, sizeof(this->NBT));
		this->NBT.NBT = NBT;
		if (!this->NBT.NBT)
			this->NBT.NBT = new NBTTagCompound("");
		NBTCompoundSanitizer sanitizer(this->NBT.NBT);
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("DataVersion", &this->NBT.DataVersion, 19133));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("Level", &this->NBT.Level));
		sanitizer.sanitize();
		sanitizer = NBTCompoundSanitizer(this->NBT.Level);
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("xPos", &this->NBT.xPos, this->x));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("zPos", &this->NBT.zPos, this->z));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryLong("LastUpdate", &this->NBT.LastUpdate, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("LightPopulated", &this->NBT.LightPopulated, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("TerrainPopulated", &this->NBT.TerrainPopulated, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("V", &this->NBT.V, 1));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryLong("InhabitedTime", &this->NBT.InhabitedTime, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByteArray("Biomes", &this->NBT.Biomes, CHUNK_WIDTH * CHUNK_WIDTH));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByteArray("HeightMap", &this->NBT.HeightMap, CHUNK_WIDTH * CHUNK_WIDTH));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryList("Sections", &this->NBT.Sections, NBT_TAG_COMPOUND));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryList("Entities", &this->NBT.Entities, NBT_TAG_COMPOUND));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryList("TileEntities", &this->NBT.TileEntities, NBT_TAG_COMPOUND));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryList("TileTicks", &this->NBT.TileTicks, NBT_TAG_COMPOUND));
		sanitizer.sanitize();
		if (this->NBT.Biomes->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH)
		{
			this->NBT.Biomes->getValues().clear();
			this->NBT.Biomes->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH, 0);
		}
		if (this->NBT.HeightMap->getValues().size() != CHUNK_WIDTH * CHUNK_WIDTH)
		{
			this->NBT.HeightMap->getValues().clear();
			this->NBT.HeightMap->getValues().resize(CHUNK_WIDTH * CHUNK_WIDTH, 0);
		}
		bool sectionAdded = false;
		for (size_t i = 0; i < this->NBT.Sections->getValues().size(); ++i)
		{
			NBTTag *tmp = this->NBT.Sections->getValues()[i];
			bool found = false;
			NBTTagCompound *section = reinterpret_cast<NBTTagCompound*>(tmp);
			if (tmp->getType() != NBT_TAG_COMPOUND)
				goto erase3;
			for (size_t j = 0; j < section->getTags().size(); ++j)
			{
				if (section->getTags()[j]->getName().compare("Y"))
					continue;
				if (section->getTags()[j]->getType() != NBT_TAG_BYTE)
					break;
				NBTTagByte *Y = reinterpret_cast<NBTTagByte*>(section->getTags()[j]);
				if (Y->getValue() >= 0 && Y->getValue() <= 15 && !this->storages[Y->getValue()])
				{
					this->storages[Y->getValue()] = new ChunkStorage(Y->getValue());
					this->storages[Y->getValue()]->initNBT(section);
					found = true;
					sectionAdded = true;
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
		if (sectionAdded)
			regenerateLightMapRec();
	}

	void Chunk::setGenerated(bool generated)
	{
		this->NBT.TerrainPopulated->setValue(generated ? 1 : 0);
	}

	bool Chunk::isGenerated()
	{
		return this->NBT.TerrainPopulated->getValue();
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
