#include "BlockBase.h"
#include "World/ChunkTessellator.h"
#include "World/ChunkBlock.h"

namespace voxel
{

	static const float texSize = 1. / 16;

	BlockBase::BlockBase(uint8_t id)
	: Block(id)
	{
		setTex(0, 0);
	}

	void BlockBase::draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)chunk;
		glm::vec3 color(1);
		std::vector<glm::vec2> &texCoords = tessellator.texCoords;
		std::vector<glm::vec3> &vertexes = tessellator.vertexes;
		std::vector<glm::vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		glm::vec3 org(pos);
		glm::vec3 dst(pos + 1.f);
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			glm::vec2 texOrg(this->texFrontX, this->texFrontY);
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
			glm::vec2 texOrg(texLeftX, texLeftY);
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
			glm::vec2 texOrg(texLeftX, texLeftY);
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
			glm::vec2 texOrg(texRightX, texRightY);
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
			glm::vec2 texOrg(texTopX, texTopY);
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
			glm::vec2 texOrg(texBotX, texBotY);
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

	BlockBase *BlockBase::setTexSide(float texSideX, float texSideY)
	{
		setTexSideX(texSideX);
		setTexSideY(texSideY);
		return (this);
	}

	BlockBase *BlockBase::setTexSideX(float texSideX)
	{
		setTexFrontX(texSideX);
		setTexRightX(texSideX);
		setTexBackX(texSideX);
		setTexLeftX(texSideX);
		return (this);
	}

	BlockBase *BlockBase::setTexSideY(float texSideY)
	{
		setTexFrontY(texSideY);
		setTexRightY(texSideY);
		setTexBackY(texSideY);
		setTexLeftY(texSideY);
		return (this);
	}

	BlockBase *BlockBase::setTexFront(float texFrontX, float texFrontY)
	{
		setTexFrontX(texFrontX);
		setTexFrontY(texFrontY);
		return (this);
	}

	BlockBase *BlockBase::setTexFrontX(float texFrontX)
	{
		this->texFrontX = texFrontX;
		return (this);
	}

	BlockBase *BlockBase::setTexFrontY(float texFrontY)
	{
		this->texFrontY = texFrontY;
		return (this);
	}

	BlockBase *BlockBase::setTexRight(float texRightX, float texRightY)
	{
		setTexRightX(texRightX);
		setTexRightY(texRightY);
		return (this);
	}

	BlockBase *BlockBase::setTexRightX(float texRightX)
	{
		this->texRightX = texRightX;
		return (this);
	}

	BlockBase *BlockBase::setTexRightY(float texRightY)
	{
		this->texRightY = texRightY;
		return (this);
	}

	BlockBase *BlockBase::setTexBack(float texBackX, float texBackY)
	{
		setTexBackX(texBackX);
		setTexBackY(texBackY);
		return (this);
	}

	BlockBase *BlockBase::setTexBackX(float texBackX)
	{
		this->texBackX = texBackX;
		return (this);
	}

	BlockBase *BlockBase::setTexBackY(float texBackY)
	{
		this->texBackY = texBackY;
		return (this);
	}

	BlockBase *BlockBase::setTexLeft(float texLeftX, float texLeftY)
	{
		setTexLeftX(texLeftX);
		setTexLeftY(texLeftY);
		return (this);
	}

	BlockBase *BlockBase::setTexLeftX(float texLeftX)
	{
		this->texLeftX = texLeftX;
		return (this);
	}

	BlockBase *BlockBase::setTexLeftY(float texLeftY)
	{
		this->texLeftY = texLeftY;
		return (this);
	}

	BlockBase *BlockBase::setTexTop(float texTopX, float texTopY)
	{
		setTexTopX(texTopX);
		setTexTopY(texTopY);
		return (this);
	}

	BlockBase *BlockBase::setTexTopX(float texTopX)
	{
		this->texTopX = texTopX;
		return (this);
	}

	BlockBase *BlockBase::setTexTopY(float texTopY)
	{
		this->texTopY = texTopY;
		return (this);
	}

	BlockBase *BlockBase::setTexBot(float texBotX, float texBotY)
	{
		setTexBotX(texBotX);
		setTexBotY(texBotY);
		return (this);
	}

	BlockBase *BlockBase::setTexBotX(float texBotX)
	{
		this->texBotX = texBotX;
		return (this);
	}

	BlockBase *BlockBase::setTexBotY(float texBotY)
	{
		this->texBotY = texBotY;
		return (this);
	}

	BlockBase *BlockBase::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return (this);
	}

	BlockBase *BlockBase::setTexX(float texX)
	{
		setTexSideX(texX);
		setTexTopX(texX);
		setTexBotX(texX);
		return (this);
	}

	BlockBase *BlockBase::setTexY(float texY)
	{
		setTexSideY(texY);
		setTexTopY(texY);
		setTexBotY(texY);
		return (this);
	}

}
