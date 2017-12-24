#include "BodyPart.h"
#include "World/World.h"
#include "Main.h"

namespace voxel
{

	BodyPart::BodyPart(Vec3 pos, Vec3 size, Vec2 uv)
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
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, positions, sizeof(positions), GL_FLOAT, 3, GL_STATIC_DRAW);
		const Vec2 texSize(64, 32);
		Vec2 texCoords[24];
		//Front
		texCoords[0]  = Vec2((uv.x + size.z         ) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		texCoords[1]  = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		texCoords[2]  = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y + size.z         ) / texSize.y);
		texCoords[3]  = Vec2((uv.x + size.z         ) / texSize.x, (uv.y + size.z         ) / texSize.y);
		//Back
		texCoords[4]  = Vec2((uv.x + size.z * 2 + size.x * 2) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		texCoords[5]  = Vec2((uv.x + size.z * 2 + size.x * 1) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		texCoords[6]  = Vec2((uv.x + size.z * 2 + size.x * 1) / texSize.x, (uv.y + size.z         ) / texSize.y);
		texCoords[7]  = Vec2((uv.x + size.z * 2 + size.x * 2) / texSize.x, (uv.y + size.z         ) / texSize.y);
		//Left
		texCoords[8]  = Vec2((uv.x         ) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		texCoords[9]  = Vec2((uv.x         ) / texSize.x, (uv.y + size.z         ) / texSize.y);
		texCoords[10] = Vec2((uv.x + size.z) / texSize.x, (uv.y + size.z         ) / texSize.y);
		texCoords[11] = Vec2((uv.x + size.z) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		//Right
		texCoords[12] = Vec2((uv.x + size.x + size.z * 2) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		texCoords[13] = Vec2((uv.x + size.x + size.z * 2) / texSize.x, (uv.y + size.z         ) / texSize.y);
		texCoords[14] = Vec2((uv.x + size.x + size.z * 1) / texSize.x, (uv.y + size.z         ) / texSize.y);
		texCoords[15] = Vec2((uv.x + size.x + size.z * 1) / texSize.x, (uv.y + size.z + size.y) / texSize.y);
		//Up
		texCoords[16] = Vec2((uv.x + size.z         ) / texSize.x, (uv.y         ) / texSize.y);
		texCoords[17] = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y         ) / texSize.y);
		texCoords[18] = Vec2((uv.x + size.z + size.x) / texSize.x, (uv.y + size.z) / texSize.y);
		texCoords[19] = Vec2((uv.x + size.z         ) / texSize.x, (uv.y + size.z) / texSize.y);
		//Down
		texCoords[20] = Vec2((uv.x + size.z + size.x * 1) / texSize.x, (uv.y + size.z * 1) / texSize.y);
		texCoords[21] = Vec2((uv.x + size.z + size.x * 2) / texSize.x, (uv.y + size.z * 1) / texSize.y);
		texCoords[22] = Vec2((uv.x + size.z + size.x * 2) / texSize.x, (uv.y             ) / texSize.y);
		texCoords[23] = Vec2((uv.x + size.z + size.x * 1) / texSize.x, (uv.y             ) / texSize.y);
		this->texCoordsBuffer.setData(GL_ARRAY_BUFFER, texCoords, sizeof(texCoords), GL_FLOAT, 2, GL_STATIC_DRAW);
		GLuint indices[36];
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
		this->indicesBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), GL_UNSIGNED_INT, 1, GL_STATIC_DRAW);
	}

	void BodyPart::draw(World *world, Mat4 model)
	{
		Main::getEntityShader().program->use();
		model = Mat4::translate(model, this->pos);
		model = Mat4::rotateZ(model, this->rot.z);
		model = Mat4::rotateY(model, this->rot.y);
		model = Mat4::rotateX(model, this->rot.x);
		Mat4 mvp = world->getPlayer().getViewProjMat() * model;
		Main::getEntityShader().mvpLocation->setMat4f(mvp);
		Main::getEntityShader().mLocation->setMat4f(model);
		Main::getEntityShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		Main::getEntityShader().texCoordsLocation->setVertexBuffer(this->texCoordsBuffer);
		this->indicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	}

}
