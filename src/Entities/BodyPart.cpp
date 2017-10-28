#include "BodyPart.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

namespace voxel
{

	BodyPart::BodyPart(glm::vec3 pos, glm::vec3 size, glm::vec2 uv)
	{
		glm::vec3 positions[24];
		//Front
		positions[0]  = glm::vec3(pos.x         , pos.y         , pos.z + size.z);
		positions[1]  = glm::vec3(pos.x + size.x, pos.y         , pos.z + size.z);
		positions[2]  = glm::vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
		positions[3]  = glm::vec3(pos.x         , pos.y + size.y, pos.z + size.z);
		//Back
		positions[4]  = glm::vec3(pos.x         , pos.y         , pos.z);
		positions[5]  = glm::vec3(pos.x + size.x, pos.y         , pos.z);
		positions[6]  = glm::vec3(pos.x + size.x, pos.y + size.y, pos.z);
		positions[7]  = glm::vec3(pos.x         , pos.y + size.y, pos.z);
		//Left
		positions[8]  = glm::vec3(pos.x         , pos.y         , pos.z);
		positions[9]  = glm::vec3(pos.x         , pos.y + size.y, pos.z);
		positions[10] = glm::vec3(pos.x         , pos.y + size.y, pos.z + size.z);
		positions[11] = glm::vec3(pos.x         , pos.y         , pos.z + size.z);
		//Right
		positions[12] = glm::vec3(pos.x + size.x, pos.y         , pos.z);
		positions[13] = glm::vec3(pos.x + size.x, pos.y + size.y, pos.z);
		positions[14] = glm::vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
		positions[15] = glm::vec3(pos.x + size.x, pos.y         , pos.z + size.z);
		//Up
		positions[16] = glm::vec3(pos.x         , pos.y + size.y, pos.z);
		positions[17] = glm::vec3(pos.x + size.x, pos.y + size.y, pos.z);
		positions[18] = glm::vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z);
		positions[19] = glm::vec3(pos.x         , pos.y + size.y, pos.z + size.z);
		//Down
		positions[20] = glm::vec3(pos.x         , pos.y         , pos.z);
		positions[21] = glm::vec3(pos.x + size.x, pos.y         , pos.z);
		positions[22] = glm::vec3(pos.x + size.x, pos.y         , pos.z + size.z);
		positions[23] = glm::vec3(pos.x         , pos.y         , pos.z + size.z);
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, positions, sizeof(positions), GL_FLOAT, 3, GL_STATIC_DRAW);
		glm::vec2 texCoords[24];
		//Front
		texCoords[0]  = glm::vec2((uv.x + size.z         ) / 64., (uv.y + size.z + size.y) / 32.);
		texCoords[1]  = glm::vec2((uv.x + size.z + size.x) / 64., (uv.y + size.z + size.y) / 32.);
		texCoords[2]  = glm::vec2((uv.x + size.z + size.x) / 64., (uv.y + size.z         ) / 32.);
		texCoords[3]  = glm::vec2((uv.x + size.z         ) / 64., (uv.y + size.z         ) / 32.);
		//Back
		texCoords[4]  = glm::vec2((uv.x + size.z * 2 + size.x * 2) / 64., (uv.y + size.z + size.y) / 32.);
		texCoords[5]  = glm::vec2((uv.x + size.z * 2 + size.x * 1) / 64., (uv.y + size.z + size.y) / 32.);
		texCoords[6]  = glm::vec2((uv.x + size.z * 2 + size.x * 1) / 64., (uv.y + size.z         ) / 32.);
		texCoords[7]  = glm::vec2((uv.x + size.z * 2 + size.x * 2) / 64., (uv.y + size.z         ) / 32.);
		//Left
		texCoords[8]  = glm::vec2((uv.x         ) / 64., (uv.y + size.z + size.y) / 32.);
		texCoords[9]  = glm::vec2((uv.x         ) / 64., (uv.y + size.z         ) / 32.);
		texCoords[10] = glm::vec2((uv.x + size.z) / 64., (uv.y + size.z         ) / 32.);
		texCoords[11] = glm::vec2((uv.x + size.z) / 64., (uv.y + size.z + size.y) / 32.);
		//Right
		texCoords[12] = glm::vec2((uv.x + size.x + size.z * 2) / 64., (uv.y + size.z + size.y) / 32.);
		texCoords[13] = glm::vec2((uv.x + size.x + size.z * 2) / 64., (uv.y + size.z         ) / 32.);
		texCoords[14] = glm::vec2((uv.x + size.x + size.z * 1) / 64., (uv.y + size.z         ) / 32.);
		texCoords[15] = glm::vec2((uv.x + size.x + size.z * 1) / 64., (uv.y + size.z + size.y) / 32.);
		//Up
		texCoords[16] = glm::vec2((uv.x + size.z         ) / 64., (uv.y         ) / 32.);
		texCoords[17] = glm::vec2((uv.x + size.z + size.x) / 64., (uv.y         ) / 32.);
		texCoords[18] = glm::vec2((uv.x + size.z + size.x) / 64., (uv.y + size.z) / 32.);
		texCoords[19] = glm::vec2((uv.x + size.z         ) / 64., (uv.y + size.z) / 32.);
		//Down
		texCoords[20] = glm::vec2((uv.x + size.z + size.x * 1) / 64., (uv.y + size.z * 1) / 32.);
		texCoords[21] = glm::vec2((uv.x + size.z + size.x * 2) / 64., (uv.y + size.z * 1) / 32.);
		texCoords[22] = glm::vec2((uv.x + size.z + size.x * 2) / 64., (uv.y             ) / 32.);
		texCoords[23] = glm::vec2((uv.x + size.z + size.x * 1) / 64., (uv.y             ) / 32.);
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

	void BodyPart::draw(World *world, glm::mat4 model)
	{
		model = glm::translate(model, this->pos);
		model = glm::rotate(model, this->rot.z, glm::vec3(0, 0, 1));
		model = glm::rotate(model, this->rot.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, this->rot.x, glm::vec3(1, 0, 0));
		glm::mat4 mvp = world->getPlayer().getViewProjMat() * model;
		Main::getEntityShader().mvpLocation->setMat4f(mvp);
		Main::getEntityShader().mLocation->setMat4f(model);
		Main::getEntityShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		Main::getEntityShader().texCoordsLocation->setVertexBuffer(this->texCoordsBuffer);
		this->indicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	}

}
