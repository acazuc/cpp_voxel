#include "BlockSapling.h"
#include "World/ChunkTessellator.h"
#include "World/ChunkBlock.h"
#include "World/Chunk.h"

namespace voxel
{

	static const float texSize = 1. / 16;

	BlockSapling::BlockSapling(uint8_t id)
	: Block(id)
	{
		setAABB(AABB(glm::vec3(.1, 0, .1), glm::vec3(.9, .8, .9)));
	}
	
	void BlockSapling::draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)visibleFaces;
		(void)lights;
		//pos of 0 make segfault on dropped block
		glm::vec3 color(ChunkBlock::getLightValue(chunk->getLight(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ())));
		float diff = (1 - 0.707) / 2 * BLOCK_SIZE;
		std::vector<glm::vec2> &texCoords = tessellator.texCoords;
		std::vector<glm::vec3> &vertexes = tessellator.vertexes;
		std::vector<glm::vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		glm::vec3 org(pos.x + diff, pos.y, pos.z + diff);
		glm::vec3 dst(pos.x + BLOCK_SIZE - diff, pos.y + BLOCK_SIZE, pos.z + BLOCK_SIZE - diff);
		glm::vec2 texOrg(this->texX, this->texY);
		glm::vec2 texDst(texOrg);
		texDst += texSize;
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(org.x, org.y, org.z));
		texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(dst.x, org.y, dst.z));
		texCoords.push_back(glm::vec2(texDst.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
		texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(org.x, dst.y, org.z));
		texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
		colors.push_back(color);
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 1);
		currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(org.x, org.y, org.z));
		texCoords.push_back(glm::vec2(texDst.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(dst.x, org.y, dst.z));
		texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
		texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(org.x, dst.y, org.z));
		texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
		colors.push_back(color);
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 3);
		currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(dst.x, org.y, org.z));
		texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
		texCoords.push_back(glm::vec2(texDst.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(org.x, dst.y, dst.z));
		texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(dst.x, dst.y, org.z));
		texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
		colors.push_back(color);
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 1);
		currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(dst.x, org.y, org.z));
		texCoords.push_back(glm::vec2(texDst.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
		texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(org.x, dst.y, dst.z));
		texCoords.push_back(glm::vec2(texOrg.x, texOrg.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(dst.x, dst.y, org.z));
		texCoords.push_back(glm::vec2(texDst.x, texOrg.y));
		colors.push_back(color);
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 3);
	}

	float BlockSapling::getDestroyTexX()
	{
		return (this->texX);
	}

	float BlockSapling::getDestroyTexY()
	{
		return (this->texY);
	}

	BlockSapling *BlockSapling::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return (this);
	}

	BlockSapling *BlockSapling::setTexX(float texX)
	{
		this->texX = texX;
		return (this);
	}

	BlockSapling *BlockSapling::setTexY(float texY)
	{
		this->texY = texY;
		return (this);
	}

}
