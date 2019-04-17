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

	void BlockBase::draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)chunk;
		Vec3 color(1);
		std::vector<Vec2> &uvs = tessellator.uvs;
		std::vector<Vec3> &positions = tessellator.positions;
		std::vector<Vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		Vec3 org(pos);
		Vec3 dst(pos + 1.f);
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			Vec2 texOrg(this->texFrontX, this->texFrontY);
			Vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = positions.size();
			Vec3 col = color * FRONT_COLOR_FACTOR;
			positions.push_back(Vec3(org.x, org.y, dst.z));
			uvs.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F1P1]);
			positions.push_back(Vec3(org.x, dst.y, dst.z));
			uvs.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F1P2]);
			positions.push_back(Vec3(dst.x, dst.y, dst.z));
			uvs.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F1P3]);
			positions.push_back(Vec3(dst.x, org.y, dst.z));
			uvs.push_back(Vec2(texDst.x, texDst.y));
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
			Vec2 texOrg(this->texLeftX, this->texLeftY);
			Vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = positions.size();
			Vec3 col = color * BACK_COLOR_FACTOR;
			positions.push_back(Vec3(org.x, org.y, org.z));
			uvs.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F2P1]);
			positions.push_back(Vec3(org.x, dst.y, org.z));
			uvs.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F2P2]);
			positions.push_back(Vec3(dst.x, dst.y, org.z));
			uvs.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F2P3]);
			positions.push_back(Vec3(dst.x, org.y, org.z));
			uvs.push_back(Vec2(texOrg.x, texDst.y));
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
			Vec2 texOrg(this->texLeftX, this->texLeftY);
			Vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = positions.size();
			Vec3 col = color * LEFT_COLOR_FACTOR;
			positions.push_back(Vec3(org.x, org.y, org.z));
			uvs.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F3P1]);
			positions.push_back(Vec3(org.x, dst.y, org.z));
			uvs.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F3P2]);
			positions.push_back(Vec3(org.x, dst.y, dst.z));
			uvs.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F3P3]);
			positions.push_back(Vec3(org.x, org.y, dst.z));
			uvs.push_back(Vec2(texDst.x, texDst.y));
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
			Vec2 texOrg(this->texRightX, this->texRightY);
			Vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = positions.size();
			Vec3 col = color * RIGHT_COLOR_FACTOR;
			positions.push_back(Vec3(dst.x, org.y, org.z));
			uvs.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F4P1]);
			positions.push_back(Vec3(dst.x, dst.y, org.z));
			uvs.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F4P2]);
			positions.push_back(Vec3(dst.x, dst.y, dst.z));
			uvs.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F4P3]);
			positions.push_back(Vec3(dst.x, org.y, dst.z));
			uvs.push_back(Vec2(texOrg.x, texDst.y));
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
			Vec2 texOrg(this->texTopX, this->texTopY);
			Vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = positions.size();
			Vec3 col = color * UP_COLOR_FACTOR;
			positions.push_back(Vec3(org.x, dst.y, dst.z));
			uvs.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F5P1]);
			positions.push_back(Vec3(org.x, dst.y, org.z));
			uvs.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F5P2]);
			positions.push_back(Vec3(dst.x, dst.y, org.z));
			uvs.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F5P3]);
			positions.push_back(Vec3(dst.x, dst.y, dst.z));
			uvs.push_back(Vec2(texDst.x, texOrg.y));
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
			Vec2 texOrg(this->texBotX, this->texBotY);
			Vec2 texDst(texOrg);
			texDst += texSize;
			GLuint currentIndice = positions.size();
			Vec3 col = color * DOWN_COLOR_FACTOR;
			positions.push_back(Vec3(org.x, org.y, dst.z));
			uvs.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F6P1]);
			positions.push_back(Vec3(org.x, org.y, org.z));
			uvs.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F6P2]);
			positions.push_back(Vec3(dst.x, org.y, org.z));
			uvs.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F6P3]);
			positions.push_back(Vec3(dst.x, org.y, dst.z));
			uvs.push_back(Vec2(texDst.x, texOrg.y));
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

	float BlockBase::getDestroyTexX()
	{
		uint8_t nb = rand() / (RAND_MAX / 6.);
		if (nb == 0)
			return this->texLeftX;
		if (nb == 1)
			return this->texRightX;
		if (nb == 3)
			return this->texFrontX;
		if (nb == 4)
			return this->texBackX;
		if (nb == 5)
			return this->texTopX;
		return this->texBotX;
	}

	float BlockBase::getDestroyTexY()
	{
		uint8_t nb = rand() / (RAND_MAX / 6.);
		if (nb == 0)
			return this->texLeftY;
		if (nb == 1)
			return this->texRightY;
		if (nb == 3)
			return this->texFrontY;
		if (nb == 4)
			return this->texBackY;
		if (nb == 5)
			return this->texTopY;
		return this->texBotY;
	}

	BlockBase *BlockBase::setTexSide(float texSideX, float texSideY)
	{
		setTexSideX(texSideX);
		setTexSideY(texSideY);
		return this;
	}

	BlockBase *BlockBase::setTexSideX(float texSideX)
	{
		setTexFrontX(texSideX);
		setTexRightX(texSideX);
		setTexBackX(texSideX);
		setTexLeftX(texSideX);
		return this;
	}

	BlockBase *BlockBase::setTexSideY(float texSideY)
	{
		setTexFrontY(texSideY);
		setTexRightY(texSideY);
		setTexBackY(texSideY);
		setTexLeftY(texSideY);
		return this;
	}

	BlockBase *BlockBase::setTexFront(float texFrontX, float texFrontY)
	{
		setTexFrontX(texFrontX);
		setTexFrontY(texFrontY);
		return this;
	}

	BlockBase *BlockBase::setTexFrontX(float texFrontX)
	{
		this->texFrontX = texFrontX;
		return this;
	}

	BlockBase *BlockBase::setTexFrontY(float texFrontY)
	{
		this->texFrontY = texFrontY;
		return this;
	}

	BlockBase *BlockBase::setTexRight(float texRightX, float texRightY)
	{
		setTexRightX(texRightX);
		setTexRightY(texRightY);
		return this;
	}

	BlockBase *BlockBase::setTexRightX(float texRightX)
	{
		this->texRightX = texRightX;
		return this;
	}

	BlockBase *BlockBase::setTexRightY(float texRightY)
	{
		this->texRightY = texRightY;
		return this;
	}

	BlockBase *BlockBase::setTexBack(float texBackX, float texBackY)
	{
		setTexBackX(texBackX);
		setTexBackY(texBackY);
		return this;
	}

	BlockBase *BlockBase::setTexBackX(float texBackX)
	{
		this->texBackX = texBackX;
		return this;
	}

	BlockBase *BlockBase::setTexBackY(float texBackY)
	{
		this->texBackY = texBackY;
		return this;
	}

	BlockBase *BlockBase::setTexLeft(float texLeftX, float texLeftY)
	{
		setTexLeftX(texLeftX);
		setTexLeftY(texLeftY);
		return this;
	}

	BlockBase *BlockBase::setTexLeftX(float texLeftX)
	{
		this->texLeftX = texLeftX;
		return this;
	}

	BlockBase *BlockBase::setTexLeftY(float texLeftY)
	{
		this->texLeftY = texLeftY;
		return this;
	}

	BlockBase *BlockBase::setTexTop(float texTopX, float texTopY)
	{
		setTexTopX(texTopX);
		setTexTopY(texTopY);
		return this;
	}

	BlockBase *BlockBase::setTexTopX(float texTopX)
	{
		this->texTopX = texTopX;
		return this;
	}

	BlockBase *BlockBase::setTexTopY(float texTopY)
	{
		this->texTopY = texTopY;
		return this;
	}

	BlockBase *BlockBase::setTexBot(float texBotX, float texBotY)
	{
		setTexBotX(texBotX);
		setTexBotY(texBotY);
		return this;
	}

	BlockBase *BlockBase::setTexBotX(float texBotX)
	{
		this->texBotX = texBotX;
		return this;
	}

	BlockBase *BlockBase::setTexBotY(float texBotY)
	{
		this->texBotY = texBotY;
		return this;
	}

	BlockBase *BlockBase::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return this;
	}

	BlockBase *BlockBase::setTexX(float texX)
	{
		setTexSideX(texX);
		setTexTopX(texX);
		setTexBotX(texX);
		return this;
	}

	BlockBase *BlockBase::setTexY(float texY)
	{
		setTexSideY(texY);
		setTexTopY(texY);
		setTexBotY(texY);
		return this;
	}

}
