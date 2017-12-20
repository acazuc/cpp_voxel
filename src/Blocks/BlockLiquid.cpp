#include "BlockLiquid.h"
#include "World/ChunkTessellator.h"
#include "World/ChunkBlock.h"

namespace voxel
{

	static const float texSize = 1. / 16;

	BlockLiquid::BlockLiquid(uint8_t id)
	: Block(id)
	{
		//Empty
	}

	void BlockLiquid::draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)chunk;
		glm::vec3 color(1);
		std::vector<glm::vec2> &texCoords = tessellator.texCoords;
		std::vector<glm::vec3> &vertexes = tessellator.vertexes;
		std::vector<glm::vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		glm::vec3 org(pos);
		float height = .9f;
		glm::vec3 dst(pos + 1.f);
		dst.y = pos.y + height;
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			glm::vec2 texOrg(this->texX, this->texY);
			glm::vec2 texDst(texOrg);
			texDst += texSize * height;
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
			if (lights[F1P2] + lights[F1P4] > lights[F1P1] + lights[F1P3])
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
			glm::vec2 texOrg(this->texX, this->texY);
			glm::vec2 texDst(texOrg);
			texDst += texSize * height;
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
			if (lights[F2P2] + lights[F2P4] <= lights[F2P1] + lights[F2P3])
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
			glm::vec2 texOrg(this->texX, this->texY);
			glm::vec2 texDst(texOrg);
			texDst += texSize * height;
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
			if (lights[F3P2] + lights[F3P4] > lights[F3P1] + lights[F3P3])
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
			glm::vec2 texOrg(this->texX, this->texY);
			glm::vec2 texDst(texOrg);
			texDst += texSize * height;
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
			if (lights[F4P2] + lights[F4P4] < lights[F4P1] + lights[F4P3])
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
			glm::vec2 texOrg(this->texX, this->texY);
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
			if (lights[F5P2] + lights[F5P4] > lights[F5P1] + lights[F5P3])
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
			glm::vec2 texOrg(this->texX, this->texY);
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
			if (lights[F6P2] + lights[F6P4] < lights[F6P1] + lights[F6P3])
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

	BlockLiquid *BlockLiquid::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return (this);
	}

	BlockLiquid *BlockLiquid::setTexX(float texX)
	{
		this->texX = texX;
		return (this);
	}

	BlockLiquid *BlockLiquid::setTexY(float texY)
	{
		this->texY = texY;
		return (this);
	}

}
