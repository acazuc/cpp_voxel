#include "BlockSlab.h"
#include "World/ChunkTessellator.h"
#include "World/Chunk.h"

namespace voxel
{

	static const float texSize = 1. / 16;

	BlockSlab::BlockSlab(uint8_t id)
	: BlockBase(id)
	{
		setAABB(AABB(Vec3(0, 0, 0), Vec3(1, .5, 1)));
	}

	void BlockSlab::draw(Chunk *chunk, Vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights)
	{
		(void)chunk;
		Vec3 color(1);
		std::vector<Vec2> &texCoords = tessellator.texCoords;
		std::vector<Vec3> &vertexes = tessellator.vertexes;
		std::vector<Vec3> &colors = tessellator.colors;
		std::vector<GLuint> &indices = tessellator.indices;
		Vec3 org(pos);
		float height = .5f;
		Vec3 dst(pos + 1.f);
		dst.y = pos.y + height;
		if (visibleFaces & BLOCK_FACE_FRONT)
		{
			Vec2 texOrg(this->texFrontX, this->texFrontY);
			Vec2 texDst(texOrg);
			texDst += texSize * height;
			GLuint currentIndice = vertexes.size();
			Vec3 col = color * FRONT_COLOR_FACTOR;
			vertexes.push_back(Vec3(org.x, org.y, dst.z));
			texCoords.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F1P1]);
			vertexes.push_back(Vec3(org.x, dst.y, dst.z));
			texCoords.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F1P2]);
			vertexes.push_back(Vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F1P3]);
			vertexes.push_back(Vec3(dst.x, org.y, dst.z));
			texCoords.push_back(Vec2(texDst.x, texDst.y));
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
			Vec2 texOrg(this->texBackX, this->texBackY);
			Vec2 texDst(texOrg);
			texDst += texSize * height;
			GLuint currentIndice = vertexes.size();
			Vec3 col = color * BACK_COLOR_FACTOR;
			vertexes.push_back(Vec3(org.x, org.y, org.z));
			texCoords.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F2P1]);
			vertexes.push_back(Vec3(org.x, dst.y, org.z));
			texCoords.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F2P2]);
			vertexes.push_back(Vec3(dst.x, dst.y, org.z));
			texCoords.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F2P3]);
			vertexes.push_back(Vec3(dst.x, org.y, org.z));
			texCoords.push_back(Vec2(texOrg.x, texDst.y));
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
			texDst += texSize * height;
			GLuint currentIndice = vertexes.size();
			Vec3 col = color * LEFT_COLOR_FACTOR;
			vertexes.push_back(Vec3(org.x, org.y, org.z));
			texCoords.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F3P1]);
			vertexes.push_back(Vec3(org.x, dst.y, org.z));
			texCoords.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F3P2]);
			vertexes.push_back(Vec3(org.x, dst.y, dst.z));
			texCoords.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F3P3]);
			vertexes.push_back(Vec3(org.x, org.y, dst.z));
			texCoords.push_back(Vec2(texDst.x, texDst.y));
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
			texDst += texSize * height;
			GLuint currentIndice = vertexes.size();
			Vec3 col = color * RIGHT_COLOR_FACTOR;
			vertexes.push_back(Vec3(dst.x, org.y, org.z));
			texCoords.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F4P1]);
			vertexes.push_back(Vec3(dst.x, dst.y, org.z));
			texCoords.push_back(Vec2(texDst.x, texOrg.y));
			colors.push_back(col * lights[F4P2]);
			vertexes.push_back(Vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F4P3]);
			vertexes.push_back(Vec3(dst.x, org.y, dst.z));
			texCoords.push_back(Vec2(texOrg.x, texDst.y));
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
			GLuint currentIndice = vertexes.size();
			Vec3 col = color * UP_COLOR_FACTOR;
			vertexes.push_back(Vec3(org.x, dst.y, dst.z));
			texCoords.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F5P1]);
			vertexes.push_back(Vec3(org.x, dst.y, org.z));
			texCoords.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F5P2]);
			vertexes.push_back(Vec3(dst.x, dst.y, org.z));
			texCoords.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F5P3]);
			vertexes.push_back(Vec3(dst.x, dst.y, dst.z));
			texCoords.push_back(Vec2(texDst.x, texOrg.y));
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
			GLuint currentIndice = vertexes.size();
			Vec3 col = color * DOWN_COLOR_FACTOR;
			vertexes.push_back(Vec3(org.x, org.y, dst.z));
			texCoords.push_back(Vec2(texOrg.x, texOrg.y));
			colors.push_back(col * lights[F6P1]);
			vertexes.push_back(Vec3(org.x, org.y, org.z));
			texCoords.push_back(Vec2(texOrg.x, texDst.y));
			colors.push_back(col * lights[F6P2]);
			vertexes.push_back(Vec3(dst.x, org.y, org.z));
			texCoords.push_back(Vec2(texDst.x, texDst.y));
			colors.push_back(col * lights[F6P3]);
			vertexes.push_back(Vec3(dst.x, org.y, dst.z));
			texCoords.push_back(Vec2(texDst.x, texOrg.y));
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

}
