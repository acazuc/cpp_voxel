#include "BodyPart.h"
#include "World/World.h"
#include "Main.h"

namespace voxel
{

	BodyPart::BodyPart(Vec3 pos, Vec3 size, Vec2 uv)
	: pos(pos)
	, rot(0)
	{
		Vec3 positions[24];
		//Front
		positions[0]  = Vec3(pos.x         , pos.y         , pos.z + size.z);
		positions[1]  = Vec3(pos.x + size.x, pos.y         , pos.z + size.z);
		positions[2]  = Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
		positions[3]  = Vec3(pos.x         , pos.y + size.y, pos.z + size.z);
		//Back
		positions[4]  = Vec3(pos.x         , pos.y         , pos.z);
		positions[5]  = Vec3(pos.x + size.x, pos.y         , pos.z);
		positions[6]  = Vec3(pos.x + size.x, pos.y + size.y, pos.z);
		positions[7]  = Vec3(pos.x         , pos.y + size.y, pos.z);
		//Left
		positions[8]  = Vec3(pos.x         , pos.y         , pos.z);
		positions[9]  = Vec3(pos.x         , pos.y + size.y, pos.z);
		positions[10] = Vec3(pos.x         , pos.y + size.y, pos.z + size.z);
		positions[11] = Vec3(pos.x         , pos.y         , pos.z + size.z);
		//Right
		positions[12] = Vec3(pos.x + size.x, pos.y         , pos.z);
		positions[13] = Vec3(pos.x + size.x, pos.y + size.y, pos.z);
		positions[14] = Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
		positions[15] = Vec3(pos.x + size.x, pos.y         , pos.z + size.z);
		//Up
		positions[16] = Vec3(pos.x         , pos.y + size.y, pos.z);
		positions[17] = Vec3(pos.x + size.x, pos.y + size.y, pos.z);
		positions[18] = Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
		positions[19] = Vec3(pos.x         , pos.y + size.y, pos.z + size.z);
		//Down
		positions[20] = Vec3(pos.x         , pos.y         , pos.z);
		positions[21] = Vec3(pos.x + size.x, pos.y         , pos.z);
		positions[22] = Vec3(pos.x + size.x, pos.y         , pos.z + size.z);
		positions[23] = Vec3(pos.x         , pos.y         , pos.z + size.z);
		this->positionBuffer.setData(GL_ARRAY_BUFFER, positions, sizeof(positions), GL_STATIC_DRAW);
		const Vec2 texSize(64, 32);
		Vec2 uvs[24];
		//Front
		uvs[0]  = Vec2((uv.x + size.z         ) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		uvs[1]  = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		uvs[2]  = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y + size.z         ) / texSize.y);
		uvs[3]  = Vec2((uv.x + size.z         ) / texSize.x, (uv.y + size.z         ) / texSize.y);
		//Back
		uvs[4]  = Vec2((uv.x + size.z * 2 + size.x * 2) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		uvs[5]  = Vec2((uv.x + size.z * 2 + size.x * 1) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		uvs[6]  = Vec2((uv.x + size.z * 2 + size.x * 1) / texSize.x, (uv.y + size.z         ) / texSize.y);
		uvs[7]  = Vec2((uv.x + size.z * 2 + size.x * 2) / texSize.x, (uv.y + size.z         ) / texSize.y);
		//Left
		uvs[8]  = Vec2((uv.x         ) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		uvs[9]  = Vec2((uv.x         ) / texSize.x, (uv.y + size.z         ) / texSize.y);
		uvs[10] = Vec2((uv.x + size.z) / texSize.x, (uv.y + size.z         ) / texSize.y);
		uvs[11] = Vec2((uv.x + size.z) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		//Right
		uvs[12] = Vec2((uv.x + size.x + size.z * 2) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		uvs[13] = Vec2((uv.x + size.x + size.z * 2) / texSize.x, (uv.y + size.z         ) / texSize.y);
		uvs[14] = Vec2((uv.x + size.x + size.z * 1) / texSize.x, (uv.y + size.z         ) / texSize.y);
		uvs[15] = Vec2((uv.x + size.x + size.z * 1) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		//Up
		uvs[16] = Vec2((uv.x + size.z         ) / texSize.x, (uv.y         ) / texSize.y);
		uvs[17] = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y         ) / texSize.y);
		uvs[18] = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y + size.z) / texSize.y);
		uvs[19] = Vec2((uv.x + size.z         ) / texSize.x, (uv.y + size.z) / texSize.y);
		//Down
		uvs[20] = Vec2((uv.x + size.z + size.x * 1) / texSize.x, (uv.y + size.z * 1) / texSize.y);
		uvs[21] = Vec2((uv.x + size.z + size.x * 2) / texSize.x, (uv.y + size.z * 1) / texSize.y);
		uvs[22] = Vec2((uv.x + size.z + size.x * 2) / texSize.x, (uv.y             ) / texSize.y);
		uvs[23] = Vec2((uv.x + size.z + size.x * 1) / texSize.x, (uv.y             ) / texSize.y);
		this->uvBuffer.setData(GL_ARRAY_BUFFER, uvs, sizeof(uvs), GL_STATIC_DRAW);
		GLubyte indices[36];
		//Front
		indices[0]  = 4 * 0 + 0;
		indices[1]  = 4 * 0 + 1;
		indices[2]  = 4 * 0 + 3;
		indices[3]  = 4 * 0 + 2;
		indices[4]  = 4 * 0 + 3;
		indices[5]  = 4 * 0 + 1;
		//Back
		indices[6]  = 4 * 1 + 0;
		indices[7]  = 4 * 1 + 3;
		indices[8]  = 4 * 1 + 1;
		indices[9]  = 4 * 1 + 2;
		indices[10] = 4 * 1 + 1;
		indices[11] = 4 * 1 + 3;
		//Left
		indices[12] = 4 * 2 + 0;
		indices[13] = 4 * 2 + 3;
		indices[14] = 4 * 2 + 1;
		indices[15] = 4 * 2 + 2;
		indices[16] = 4 * 2 + 1;
		indices[17] = 4 * 2 + 3;
		//Right
		indices[18] = 4 * 3 + 0;
		indices[19] = 4 * 3 + 1;
		indices[20] = 4 * 3 + 3;
		indices[21] = 4 * 3 + 2;
		indices[22] = 4 * 3 + 3;
		indices[23] = 4 * 3 + 1;
		//Up
		indices[24] = 4 * 4 + 0;
		indices[25] = 4 * 4 + 3;
		indices[26] = 4 * 4 + 1;
		indices[27] = 4 * 4 + 2;
		indices[28] = 4 * 4 + 1;
		indices[29] = 4 * 4 + 3;
		//Down
		indices[30] = 4 * 5 + 0;
		indices[31] = 4 * 5 + 1;
		indices[32] = 4 * 5 + 3;
		indices[33] = 4 * 5 + 2;
		indices[34] = 4 * 5 + 3;
		indices[35] = 4 * 5 + 1;
		this->indiceBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), GL_STATIC_DRAW);
	}

	void BodyPart::draw(World *world, Mat4 model)
	{
		Main::getEntityShader().program->use();
		model = Mat4::translate(model, this->pos);
		model = Mat4::rotateZ(model, this->rot.z);
		model = Mat4::rotateY(model, this->rot.y);
		model = Mat4::rotateX(model, this->rot.x);
		Mat4 mvp = world->getPlayer().getViewProjMat() * model;
		Main::getEntityShader().mvpLocation.setMat4f(mvp);
		Main::getEntityShader().mLocation.setMat4f(model);
		Main::getEntityShader().vertexPositionLocation.setVertexBuffer(this->positionBuffer, 3, GL_FLOAT);
		Main::getEntityShader().vertexUVLocation.setVertexBuffer(this->uvBuffer, 2, GL_FLOAT);
		this->indiceBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr);
	}

}
