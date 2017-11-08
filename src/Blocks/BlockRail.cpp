#include "BlockRail.h"
#include "World/ChunkTessellator.h"
#include "World/ChunkBlock.h"
#include "World/Chunk.h"

namespace voxel
{

	static const float texSize = 1. / 16;

	BlockRail::BlockRail(uint8_t id)
	: Block(id)
	{
		setAABB(AABB(glm::vec3(0, 0, 0), glm::vec3(1, 2. / 16, 1)));
	}
	
	void BlockRail::draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)chunk;
		(void)visibleFaces;
		(void)lights;
		glm::vec3 color(ChunkBlock::getLightValue(chunk->getLight(glm::vec3(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ()))));
		std::vector<glm::vec2> &texCoords = tessellator.texCoords;
		std::vector<glm::vec3> &vertexes = tessellator.vertexes;
		std::vector<glm::vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		glm::vec3 org(pos.x, pos.y + 1. / 16, pos.z);
		glm::vec3 dst(pos.x + BLOCK_SIZE, pos.y + 1. / 16, pos.z + BLOCK_SIZE);
		glm::vec2 texOrg(this->texX, this->texY);
		glm::vec2 texDst(texOrg);
		texDst += texSize;
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(org.x, org.y, org.z));
		texCoords.push_back(glm::vec2(texOrg.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(org.x, org.y, dst.z));
		texCoords.push_back(glm::vec2(texDst.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(glm::vec3(dst.x, dst.y, dst.z));
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
	}

	BlockRail *BlockRail::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return (this);
	}

	BlockRail *BlockRail::setTexX(float texX)
	{
		this->texX = texX;
		return (this);
	}

	BlockRail *BlockRail::setTexY(float texY)
	{
		this->texY = texY;
		return (this);
	}

}
