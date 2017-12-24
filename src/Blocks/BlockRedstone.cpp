#include "BlockRedstone.h"
#include "World/ChunkTessellator.h"
#include "World/ChunkBlock.h"
#include "World/Chunk.h"

namespace voxel
{

	static const float texSize = 1. / 16;

	BlockRedstone::BlockRedstone(uint8_t id)
	: Block(id)
	{
		setAABB(AABB(Vec3(0, 0, 0), Vec3(1, 1. / 16, 1)));
	}
	
	void BlockRedstone::draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)visibleFaces;
		(void)lights;
		Vec3 color(ChunkBlock::getLightValue(chunk->getLight(pos.x - chunk->getX(), pos.y, pos.z - chunk->getZ())));
		std::vector<Vec2> &texCoords = tessellator.texCoords;
		std::vector<Vec3> &vertexes = tessellator.vertexes;
		std::vector<Vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		Vec3 org(pos.x, pos.y + .2 / 16, pos.z);
		Vec3 dst(pos.x + BLOCK_SIZE, pos.y + .2 / 16, pos.z + BLOCK_SIZE);
		Vec2 texOrg(this->texX, this->texY);
		Vec2 texDst(texOrg);
		texDst += texSize;
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(Vec3(org.x, org.y, org.z));
		texCoords.push_back(Vec2(texOrg.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(Vec3(dst.x, org.y, org.z));
		texCoords.push_back(Vec2(texDst.x, texDst.y));
		colors.push_back(color);
		vertexes.push_back(Vec3(dst.x, dst.y, dst.z));
		texCoords.push_back(Vec2(texDst.x, texOrg.y));
		colors.push_back(color);
		vertexes.push_back(Vec3(org.x, dst.y, dst.z));
		texCoords.push_back(Vec2(texOrg.x, texOrg.y));
		colors.push_back(color);
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 3);
	}

	float BlockRedstone::getDestroyTexX()
	{
		return (this->texX);
	}

	float BlockRedstone::getDestroyTexY()
	{
		return (this->texY);
	}

	BlockRedstone *BlockRedstone::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return (this);
	}

	BlockRedstone *BlockRedstone::setTexX(float texX)
	{
		this->texX = texX;
		return (this);
	}

	BlockRedstone *BlockRedstone::setTexY(float texY)
	{
		this->texY = texY;
		return (this);
	}

}
