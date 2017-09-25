#include "ChunkBlock.h"
#include "Blocks/Blocks.h"
#include "Chunk.h"
#include "Main.h"
#include <cstring>

#define LIGHT_MIN .125
#define LIGHT_RANGE .7
#define SSAO_FACTOR 2

#define F1P1 0
#define F1P2 1
#define F1P3 2
#define F1P4 3
#define F2P1 4
#define F2P2 5
#define F2P3 6
#define F2P4 7
#define F3P1 8
#define F3P2 9
#define F3P3 10
#define F3P4 11
#define F4P1 12
#define F4P2 13
#define F4P3 14
#define F4P4 15
#define F5P1 16
#define F5P2 17
#define F5P3 18
#define F5P4 19
#define F6P1 20
#define F6P2 21
#define F6P3 22
#define F6P4 23

#define FRONT_COLOR_FACTOR .95f
#define BACK_COLOR_FACTOR .95f
#define LEFT_COLOR_FACTOR .9f
#define RIGHT_COLOR_FACTOR .9f
#define UP_COLOR_FACTOR 1.f
#define DOWN_COLOR_FACTOR .6f

#define LIGHT_SSAO_TEST(light, ssao) if (light < ssao) {light = 0;} else {light -= ssao;}

namespace voxel
{

	static const float lightsLevelsValues[] = {.03518, .04398, .05497, .06871, .08589, .10737, .13421, .16777, .20971, .26214, .32768, .4096, .512, .64, .8, 1};
	//static float lightsLevelsValues[] = {.0625, .125, .1875, .25, .3125, .375, .4375, .5, .5625, .625, .6875, .75, .8125, .875, .9375, 1};
	static const float texSize = 1. / 16;

	void ChunkBlock::fillBuffers(Chunk *chunk, glm::vec3 &pos, ChunkTessellator &tessellator, uint8_t layer)
	{
		if (this->type == 0)
			return;
		Block *blockModel = Blocks::getBlock(this->type);
		if (!blockModel)
			return;
		if (blockModel->getLayer() != layer)
			return;
		uint8_t visibleFaces;
		calcVisibleFaces(chunk, pos, visibleFaces);
		if (!visibleFaces)
			return;
		bool blocksTransparent[27];
		int8_t blocksLights[27];
		{
			int8_t i = 0;
			for (int8_t x = -1; x <= 1; ++x)
			{
				for (int8_t y = -1; y <= 1; ++y)
				{
					for (int8_t z = -1; z <= 1; ++z)
					{
						blocksTransparent[i] = calcTransparent(chunk, pos, x, y, z);
						blocksLights[i++] = calcLightLevel(chunk, pos, x, y, z);
					}
				}
			}
		}
		BlockLightsLevels lightsLevels;
		initLightsLevels(lightsLevels, visibleFaces, blocksLights);
		if (Main::getSsao() && blockModel->isSolid())
			calcAmbientOcclusion(pos, lightsLevels, visibleFaces, blocksTransparent);
		glm::vec3 color(1, 1, 1);
		float lights[24];
		smoothLights(lights, visibleFaces, lightsLevels, blocksTransparent, blocksLights);
		std::vector<glm::vec2> &texCoords = tessellator.texCoords;
		std::vector<glm::vec3> &vertexes = tessellator.vertexes;
		std::vector<glm::vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		if (!blockModel->isFullCube())
		{
			float diff = (1 - 0.707) / 2 * BLOCK_SIZE;
			glm::vec3 org(pos.x + diff, pos.y, pos.z + diff);
			glm::vec3 dst(pos.x + BLOCK_SIZE - diff, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE - diff);
			glm::vec2 texOrg(blockModel->getTexFrontX(), blockModel->getTexFrontY());
			glm::vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(org.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(dst.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
			currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(org.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(dst.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(dst.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
			currentIndice = vertexes.size();
			vertexes.push_back(glm::vec3(dst.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(color * lights[F1P1]);
			indices.push_back(currentIndice + 0);
			indices.push_back(currentIndice + 3);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 2);
			indices.push_back(currentIndice + 1);
			indices.push_back(currentIndice + 3);
			return;
		}
		glm::vec3 org(pos);
		glm::vec3 dst(pos);
		dst += BLOCK_SIZE;
		if (this->type == 8 || this->type == 9)
		{
			ChunkBlock *block = chunk->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y + 1, pos.z - chunk->getZ()));
			if (!block)
				dst.y -= 2. / 16;
			else
			{
				uint8_t topType = block->getType();
				if (topType != 8 && topType != 9)
					dst.y -= 2. / 16;
			}
		}
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			glm::vec2 texOrg(blockModel->getTexFrontX(), blockModel->getTexFrontY());
			glm::vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = vertexes.size();
			glm::vec3 col = color * FRONT_COLOR_FACTOR;
			vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F1P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F1P2]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F1P3]);
			vertexes.push_back(glm::vec3(dst.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F1P4]);
			if (lightsLevels.f1p2 + lightsLevels.f1p4 > lightsLevels.f1p1 + lightsLevels.f1p3)
			{
				indices.push_back(currentIndice + 1); //1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); // 3
				indices.push_back(currentIndice + 3);
			}
			else
			{
				indices.push_back(currentIndice + 2); // 1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); //3
				indices.push_back(currentIndice + 0);
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			glm::vec2 texOrg(blockModel->getTexLeftX(), blockModel->getTexLeftY());
			glm::vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = vertexes.size();
			glm::vec3 col = color * BACK_COLOR_FACTOR;
			vertexes.push_back(glm::vec3(org.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F2P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F2P2]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F2P3]);
			vertexes.push_back(glm::vec3(dst.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F2P4]);
			if (lightsLevels.f2p2 + lightsLevels.f2p4 <= lightsLevels.f2p1 + lightsLevels.f2p3)
			{
				indices.push_back(currentIndice + 2); //1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); // 3
				indices.push_back(currentIndice + 0);
			}
			else
			{
				indices.push_back(currentIndice + 1); // 1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); //3
				indices.push_back(currentIndice + 3);
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			glm::vec2 texOrg(blockModel->getTexLeftX(), blockModel->getTexLeftY());
			glm::vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = vertexes.size();
			glm::vec3 col = color * LEFT_COLOR_FACTOR;
			vertexes.push_back(glm::vec3(org.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F3P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F3P2]);
			vertexes.push_back(glm::vec3(org.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F3P3]);
			vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F3P4]);
			if (lightsLevels.f3p2 + lightsLevels.f3p4 > lightsLevels.f3p1 + lightsLevels.f3p3)
			{
				indices.push_back(currentIndice + 1); //1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); // 3
				indices.push_back(currentIndice + 3);
			}
			else
			{
				indices.push_back(currentIndice + 2); // 1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); //3
				indices.push_back(currentIndice + 0);
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			glm::vec2 texOrg(blockModel->getTexRightX(), blockModel->getTexRightY());
			glm::vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = vertexes.size();
			glm::vec3 col = color * RIGHT_COLOR_FACTOR;
			vertexes.push_back(glm::vec3(dst.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F4P1]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F4P2]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F4P3]);
			vertexes.push_back(glm::vec3(dst.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F4P4]);
			if (lightsLevels.f4p2 + lightsLevels.f4p4 < lightsLevels.f4p1 + lightsLevels.f4p3)
			{
				indices.push_back(currentIndice + 2); //1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); // 3
				indices.push_back(currentIndice + 0);
			}
			else
			{
				indices.push_back(currentIndice + 1); // 1
				indices.push_back(currentIndice + 3); //23
				indices.push_back(currentIndice + 0);
				indices.push_back(currentIndice + 1); //21
				indices.push_back(currentIndice + 2); //3
				indices.push_back(currentIndice + 3);
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			glm::vec2 texOrg(blockModel->getTexTopX(), blockModel->getTexTopY());
			glm::vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = vertexes.size();
			glm::vec3 col = color * UP_COLOR_FACTOR;
			vertexes.push_back(glm::vec3(org.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F5P1]);
			vertexes.push_back(glm::vec3(org.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F5P2]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F5P3]);
			vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F5P4]);
			if (lightsLevels.f5p2 + lightsLevels.f5p4 > lightsLevels.f5p1 + lightsLevels.f5p3)
			{
				indices.push_back(currentIndice + 1); //1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); // 3
				indices.push_back(currentIndice + 3);
			}
			else
			{
				indices.push_back(currentIndice + 2); // 1
				indices.push_back(currentIndice + 0); //23
				indices.push_back(currentIndice + 3);
				indices.push_back(currentIndice + 2); //21
				indices.push_back(currentIndice + 1); //3
				indices.push_back(currentIndice + 0);
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			glm::vec2 texOrg(blockModel->getTexBotX(), blockModel->getTexBotY());
			glm::vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = vertexes.size();
			glm::vec3 col = color * DOWN_COLOR_FACTOR;
			vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F6P1]);
			vertexes.push_back(glm::vec3(org.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F6P2]);
			vertexes.push_back(glm::vec3(dst.x, org.y, org.z));
			texCoords.push_back(glm::vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F6P3]);
			vertexes.push_back(glm::vec3(dst.x, org.y, dst.z));
			texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F6P4]);
			if (lightsLevels.f6p2 + lightsLevels.f6p4 < lightsLevels.f6p1 + lightsLevels.f6p3)
			{
				indices.push_back(currentIndice + 0); //1
				indices.push_back(currentIndice + 1); //23
				indices.push_back(currentIndice + 2);
				indices.push_back(currentIndice + 3); //21
				indices.push_back(currentIndice + 0); // 3
				indices.push_back(currentIndice + 2);
			}
			else
			{
				indices.push_back(currentIndice + 3); // 1
				indices.push_back(currentIndice + 1); //23
				indices.push_back(currentIndice + 2);
				indices.push_back(currentIndice + 3); //21
				indices.push_back(currentIndice + 0); //3
				indices.push_back(currentIndice + 1);
			}
		}
	}

	bool ChunkBlock::shouldRenderFaceNear(ChunkBlock *block)
	{
		if (!block)
			return (true);
		if (!block->isTransparent())
			return (false);
		if (block->getType() == this->type)
		{
			Block *blockModel = Blocks::getBlock(this->type);
			if (!blockModel || !blockModel->isRenderSameNeighbor())
				return (false);
		}
		return (true);
	}

	void ChunkBlock::calcVisibleFaces(Chunk *chunk, glm::vec3 &pos, uint8_t &visibleFaces)
	{
		visibleFaces = 0;
		if (pos.z - chunk->getZ() == CHUNK_WIDTH - 1)
		{
			if (!chunk->getChunkZMore())
				visibleFaces |= BLOCK_FACE_FRONT;
			else if (shouldRenderFaceNear(chunk->getChunkZMore()->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y, 0))))
				visibleFaces |= BLOCK_FACE_FRONT;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() + 1))))
		{
			visibleFaces |= BLOCK_FACE_FRONT;
		}
		if (pos.z - chunk->getZ() == 0)
		{
			if (!chunk->getChunkZLess())
				visibleFaces |= BLOCK_FACE_BACK;
			else if (shouldRenderFaceNear(chunk->getChunkZLess()->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y, CHUNK_WIDTH - 1))))
				visibleFaces |= BLOCK_FACE_BACK;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ() - 1))))
		{
			visibleFaces |= BLOCK_FACE_BACK;
		}
		if (pos.x - chunk->getX() == 0)
		{
			if (!chunk->getChunkXLess())
				visibleFaces |= BLOCK_FACE_LEFT;
			else if (shouldRenderFaceNear(chunk->getChunkXLess()->getBlock(glm::vec3(CHUNK_WIDTH - 1, pos.y, pos.z - chunk->getZ()))))
				visibleFaces |= BLOCK_FACE_LEFT;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(glm::vec3(pos.x - chunk->getX() - 1, pos.y, pos.z - chunk->getZ()))))
		{
			visibleFaces |= BLOCK_FACE_LEFT;
		}
		if (pos.x - chunk->getX() == CHUNK_WIDTH - 1)
		{
			if (!chunk->getChunkXMore())
				visibleFaces |= BLOCK_FACE_RIGHT;
			else if (shouldRenderFaceNear(chunk->getChunkXMore()->getBlock(glm::vec3(0, pos.y, pos.z - chunk->getZ()))))
				visibleFaces |= BLOCK_FACE_RIGHT;
		}
		else if (shouldRenderFaceNear(chunk->getBlock(glm::vec3(pos.x - chunk->getX() + 1, pos.y, pos.z - chunk->getZ()))))
		{
			visibleFaces |= BLOCK_FACE_RIGHT;
		}
		if (pos.y == CHUNK_HEIGHT - 1)
			visibleFaces |= BLOCK_FACE_UP;
		else if (shouldRenderFaceNear(chunk->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y + 1, pos.z - chunk->getZ()))))
			visibleFaces |= BLOCK_FACE_UP;
		if (pos.y == 0)
			visibleFaces |= BLOCK_FACE_DOWN;
		else if (shouldRenderFaceNear(chunk->getBlock(glm::vec3(pos.x - chunk->getX(), pos.y - 1, pos.z - chunk->getZ()))))
			visibleFaces |= BLOCK_FACE_DOWN;
	}

	uint8_t ChunkBlock::calcLightLevel(Chunk *chunk, glm::vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return (15);
		glm::vec3 newPos(pos.x - chunk->getX() + addX, pos.y + addY, pos.z - chunk->getZ() + addZ);
		if (newPos.x < 0)
		{
			newPos.x += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXLess()))
				return (15);
		}
		else if (newPos.x >= CHUNK_WIDTH)
		{
			newPos.x -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXMore()))
				return (15);
		}
		if (newPos.z < 0)
		{
			newPos.z += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZLess()))
				return (15);
		}
		else if (newPos.z >= CHUNK_WIDTH)
		{
			newPos.z -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZMore()))
				return (15);
		}
		ChunkBlock *block = chunk->getBlock(newPos);
		if (!block)
			return (0);
		return (block->getLight());
	}

	bool ChunkBlock::calcTransparent(Chunk *chunk, glm::vec3 &pos, int8_t addX, int8_t addY, int8_t addZ)
	{
		if (pos.y + addY < 0 || pos.y + addY >= CHUNK_HEIGHT)
			return (true);
		int32_t newX = pos.x - chunk->getX() + addX;
		int32_t newY = pos.y + addY;
		int32_t newZ = pos.z - chunk->getZ() + addZ;
		if (newX < 0)
		{
			newX += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXLess()))
				return (true);
		}
		else if (newX >= CHUNK_WIDTH)
		{
			newX -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkXMore()))
				return (true);
		}
		if (newZ < 0)
		{
			newZ += CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZLess()))
				return (true);
		}
		else if (newZ >= CHUNK_WIDTH)
		{
			newZ -= CHUNK_WIDTH;
			if (!(chunk = chunk->getChunkZMore()))
				return (true);
		}
		ChunkBlock *block = chunk->getBlock(glm::vec3(newX, newY, newZ));
		if (!block)
			return (true);
		return (block->isTransparent());
	}

	void ChunkBlock::initLightsLevels(BlockLightsLevels &lights, uint8_t /*visibleFaces*/, int8_t *blocksLights)
	{
		std::memset(&lights, ((blocksLights[(1 * 3 + 1) * 3 + 1] & 0xf) << 4) | (blocksLights[(1 * 3 + 1) * 3 + 1] & 0xf), sizeof(lights));
		/*if (visibleFaces & BLOCK_FACE_FRONT)
		{
			int8_t value = blocksLights[(1 * 3 + 1) * 3 + 2];
			lights.f1p1 = value;
			lights.f1p2 = value;
			lights.f1p3 = value;
			lights.f1p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			int8_t value = blocksLights[(1 * 3 + 1) * 3 + 0];
			lights.f2p1 = value;
			lights.f2p2 = value;
			lights.f2p3 = value;
			lights.f2p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			int8_t value = blocksLights[(0 * 3 + 1) * 3 + 1];
			lights.f3p1 = value;
			lights.f3p2 = value;
			lights.f3p3 = value;
			lights.f3p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			int8_t value = blocksLights[(2 * 3 + 1) * 3 + 1];
			lights.f4p1 = value;
			lights.f4p2 = value;
			lights.f4p3 = value;
			lights.f4p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			int8_t value = blocksLights[(1 * 3 + 2) * 3 + 1];
			lights.f5p1 = value;
			lights.f5p2 = value;
			lights.f5p3 = value;
			lights.f5p4 = value;
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			int8_t value = blocksLights[(1 * 3 + 0) * 3 + 1];
			lights.f6p1 = value;
			lights.f6p2 = value;
			lights.f6p3 = value;
			lights.f6p4 = value;
		}*/
	}

	void ChunkBlock::calcAmbientOcclusion(glm::vec3 &pos, BlockLightsLevels &lights, uint8_t visibleFaces, bool *blocksTransparent)
	{
		BlockLightsLevels ssao;
		std::memset(&ssao, 0x00, sizeof(ssao));
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ssao.f1p4 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 2])
			{
				ssao.f1p4 += SSAO_FACTOR;
				ssao.f1p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ssao.f1p1 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ssao.f1p2 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 2])
			{
				ssao.f1p2 += SSAO_FACTOR;
				ssao.f1p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ssao.f1p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 2])
			{
				ssao.f1p3 += SSAO_FACTOR;
				ssao.f1p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 2])
			{
				ssao.f1p1 += SSAO_FACTOR;
				ssao.f1p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ssao.f2p4 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 0])
			{
				ssao.f2p4 += SSAO_FACTOR;
				ssao.f2p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ssao.f2p1 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ssao.f2p2 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 0])
			{
				ssao.f2p2 += SSAO_FACTOR;
				ssao.f2p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ssao.f2p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 0])
			{
				ssao.f2p3 += SSAO_FACTOR;
				ssao.f2p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 0])
			{
				ssao.f2p1 += SSAO_FACTOR;
				ssao.f2p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ssao.f3p4 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 1])
			{
				ssao.f3p4 += SSAO_FACTOR;
				ssao.f3p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ssao.f3p1 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ssao.f3p2 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 1])
			{
				ssao.f3p2 += SSAO_FACTOR;
				ssao.f3p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ssao.f3p3 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 2])
			{
				ssao.f3p3 += SSAO_FACTOR;
				ssao.f3p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 1) * 3 + 0])
			{
				ssao.f3p1 += SSAO_FACTOR;
				ssao.f3p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ssao.f4p4 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 1])
			{
				ssao.f4p4 += SSAO_FACTOR;
				ssao.f4p1 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ssao.f4p1 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ssao.f4p2 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 1])
			{
				ssao.f4p2 += SSAO_FACTOR;
				ssao.f4p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ssao.f4p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 2])
			{
				ssao.f4p3 += SSAO_FACTOR;
				ssao.f4p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 1) * 3 + 0])
			{
				ssao.f4p1 += SSAO_FACTOR;
				ssao.f4p2 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_UP && pos.y < CHUNK_HEIGHT - 1)
		{
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 0])
				ssao.f5p2 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 1])
			{
				ssao.f5p1 += SSAO_FACTOR;
				ssao.f5p2 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 2) * 3 + 2])
				ssao.f5p1 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 0])
				ssao.f5p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 1])
			{
				ssao.f5p3 += SSAO_FACTOR;
				ssao.f5p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 2) * 3 + 2])
				ssao.f5p4 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 0])
			{
				ssao.f5p2 += SSAO_FACTOR;
				ssao.f5p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(1 * 3 + 2) * 3 + 2])
			{
				ssao.f5p4 += SSAO_FACTOR;
				ssao.f5p1 += SSAO_FACTOR;
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN && pos.y > 0)
		{
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 2])
				ssao.f6p1 += SSAO_FACTOR;
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 1])
			{
				ssao.f6p1 += SSAO_FACTOR;
				ssao.f6p2 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(0 * 3 + 0) * 3 + 0])
				ssao.f6p2 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 0])
				ssao.f6p3 += SSAO_FACTOR;
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 1])
			{
				ssao.f6p3 += SSAO_FACTOR;
				ssao.f6p4 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(2 * 3 + 0) * 3 + 2])
				ssao.f6p4 += SSAO_FACTOR;
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 0])
			{
				ssao.f6p2 += SSAO_FACTOR;
				ssao.f6p3 += SSAO_FACTOR;
			}
			if (!blocksTransparent[(1 * 3 + 0) * 3 + 2])
			{
				ssao.f6p4 += SSAO_FACTOR;
				ssao.f6p1 += SSAO_FACTOR;
			}
		}
		LIGHT_SSAO_TEST(lights.f1p1, ssao.f1p1);
		LIGHT_SSAO_TEST(lights.f1p2, ssao.f1p2);
		LIGHT_SSAO_TEST(lights.f1p3, ssao.f1p3);
		LIGHT_SSAO_TEST(lights.f1p4, ssao.f1p4);
		LIGHT_SSAO_TEST(lights.f2p1, ssao.f2p1);
		LIGHT_SSAO_TEST(lights.f2p2, ssao.f2p2);
		LIGHT_SSAO_TEST(lights.f2p3, ssao.f2p3);
		LIGHT_SSAO_TEST(lights.f2p4, ssao.f2p4);
		LIGHT_SSAO_TEST(lights.f3p1, ssao.f3p1);
		LIGHT_SSAO_TEST(lights.f3p2, ssao.f3p2);
		LIGHT_SSAO_TEST(lights.f3p3, ssao.f3p3);
		LIGHT_SSAO_TEST(lights.f3p4, ssao.f3p4);
		LIGHT_SSAO_TEST(lights.f4p1, ssao.f4p1);
		LIGHT_SSAO_TEST(lights.f4p2, ssao.f4p2);
		LIGHT_SSAO_TEST(lights.f4p3, ssao.f4p3);
		LIGHT_SSAO_TEST(lights.f4p4, ssao.f4p4);
		LIGHT_SSAO_TEST(lights.f5p1, ssao.f5p1);
		LIGHT_SSAO_TEST(lights.f5p2, ssao.f5p2);
		LIGHT_SSAO_TEST(lights.f5p3, ssao.f5p3);
		LIGHT_SSAO_TEST(lights.f5p4, ssao.f5p4);
		LIGHT_SSAO_TEST(lights.f6p1, ssao.f6p1);
		LIGHT_SSAO_TEST(lights.f6p2, ssao.f6p2);
		LIGHT_SSAO_TEST(lights.f6p3, ssao.f6p3);
		LIGHT_SSAO_TEST(lights.f6p4, ssao.f6p4);
	}

	void ChunkBlock::smoothLights(float *lights, uint8_t visibleFaces, BlockLightsLevels &lightsLevels, bool *blocksTransparent, int8_t *blocksLights)
	{
		Block *block = Blocks::getBlock(this->type);
		if (!Main::getSmooth() || !block->isSolid() || isTransparent())
		{
			lights[F1P1] = getLightValue(lightsLevels.f1p1);
			lights[F1P2] = getLightValue(lightsLevels.f1p2);
			lights[F1P3] = getLightValue(lightsLevels.f1p3);
			lights[F1P4] = getLightValue(lightsLevels.f1p4);
			lights[F2P1] = getLightValue(lightsLevels.f2p1);
			lights[F2P2] = getLightValue(lightsLevels.f2p2);
			lights[F2P3] = getLightValue(lightsLevels.f2p3);
			lights[F2P4] = getLightValue(lightsLevels.f2p4);
			lights[F3P1] = getLightValue(lightsLevels.f3p1);
			lights[F3P2] = getLightValue(lightsLevels.f3p2);
			lights[F3P3] = getLightValue(lightsLevels.f3p3);
			lights[F3P4] = getLightValue(lightsLevels.f3p4);
			lights[F4P1] = getLightValue(lightsLevels.f4p1);
			lights[F4P2] = getLightValue(lightsLevels.f4p2);
			lights[F4P3] = getLightValue(lightsLevels.f4p3);
			lights[F4P4] = getLightValue(lightsLevels.f4p4);
			lights[F5P1] = getLightValue(lightsLevels.f5p1);
			lights[F5P2] = getLightValue(lightsLevels.f5p2);
			lights[F5P3] = getLightValue(lightsLevels.f5p3);
			lights[F5P4] = getLightValue(lightsLevels.f5p4);
			lights[F6P1] = getLightValue(lightsLevels.f6p1);
			lights[F6P2] = getLightValue(lightsLevels.f6p2);
			lights[F6P3] = getLightValue(lightsLevels.f6p3);
			lights[F6P4] = getLightValue(lightsLevels.f6p4);
			return;
		}
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f1p1 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F1P1] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f1p4 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F1P4] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f1p3 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F1P3] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 2] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f1p2 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F1P2] = result;
			}
		}
		if (visibleFaces & BLOCK_FACE_BACK)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f2p1 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F2P1] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 0; y <= 1; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f2p4 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F2P4] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f2p3 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F2P3] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t y = 1; y <= 2; ++y)
					{
						if ((x == 1 && y == 1) || (blocksTransparent[(x * 3 + y) * 3 + 0] && !blocksTransparent[(x * 3 + y) * 3 + 1]))
						{
							totalLight += blocksLights[(x * 3 + y) * 3 + 1];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f2p2 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F2P2] = result;
			}
		}
		if (visibleFaces & BLOCK_FACE_LEFT)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f3p1 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F3P1] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f3p2 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F3P2] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f3p3 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F3P3] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(0 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f3p4 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F3P4] = result;
			}
		}
		if (visibleFaces & BLOCK_FACE_RIGHT)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f4p1 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F4P1] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f4p2 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F4P2] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 1; y <= 2; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f4p3 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F4P3] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t y = 0; y <= 1; ++y)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((z == 1 && y == 1) || (blocksTransparent[(2 * 3 + y) * 3 + z] && !blocksTransparent[(1 * 3 + y) * 3 + z]))
						{
							totalLight += blocksLights[(1 * 3 + y) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f4p4 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F4P4] = result;
			}
		}
		if (visibleFaces & BLOCK_FACE_UP)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f5p2 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F5P2] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f5p3 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F5P3] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f5p4 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F5P4] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 2) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f5p1 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F5P1] = result;
			}
		}
		if (visibleFaces & BLOCK_FACE_DOWN)
		{
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f6p2 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F6P2] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 0; z <= 1; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f6p3 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F6P3] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 1; x <= 2; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f6p4 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F6P4] = result;
			}
			{
				int8_t totalLight = 0;
				int8_t lightsPoints = 0;
				for (int8_t x = 0; x <= 1; ++x)
				{
					for (int8_t z = 1; z <= 2; ++z)
					{
						if ((x == 1 && z == 1) || (blocksTransparent[(x * 3 + 0) * 3 + z] && !blocksTransparent[(x * 3 + 1) * 3 + z]))
						{
							totalLight += blocksLights[(x * 3 + 1) * 3 + z];
							lightsPoints++;
						}
					}
				}
				int8_t average = totalLight / lightsPoints;
				float result = getLightValue(average + lightsLevels.f6p1 - blocksLights[(1 * 3 + 1) * 3 + 1]);
				lights[F6P1] = result;
			}
		}
	}

	float ChunkBlock::getLightValue(int8_t level)
	{
		level = std::max((int8_t)0, std::min((int8_t)15, level));
		return (lightsLevelsValues[level] * LIGHT_RANGE + LIGHT_MIN);
	}

	bool ChunkBlock::isTransparent()
	{
		if (this->type == 0)
			return (true);
		Block *block = Blocks::getBlock(this->type);
		if (!block)
			return (true);
		if (block->isTransparent())
			return (true);
		return (false);
	}

}
