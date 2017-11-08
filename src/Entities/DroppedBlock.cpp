#include "DroppedBlock.h"
#include "World/ChunkTessellator.h"
#include "Blocks/Blocks.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

#define DROPPED_SIZE .1

extern int64_t nanotime;

namespace voxel
{

	DroppedBlock::DroppedBlock(World &world, Chunk *chunk, uint8_t type, glm::vec3 pos, glm::vec3 vel)
	: Entity(world, NULL)
	, created(nanotime)
	, number(1)
	, type(type)
	{
		this->chunk = chunk;
		Block *block = Blocks::getBlock(type);
		if (!block)
		{
			this->deleted = true;
			return;
		}
		this->gravity = .04;
		setSize(glm::vec3(.3, .5, .3));
		setPos(pos);
		this->posDst = vel;
		ChunkTessellator tessellator;
		float tmp[24];
		for (uint8_t i = 0; i < 24; ++i)
			tmp[i] = ChunkBlock::getLightValue(0xf);
		block->draw(this->chunk, glm::vec3(0, 0, 0), tessellator, 0xff, tmp);
		this->texCoordsBuffer.setData(GL_ARRAY_BUFFER, tessellator.texCoords.data(), sizeof(glm::vec2) * tessellator.texCoords.size(), GL_FLOAT, 2, GL_STATIC_DRAW);
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, tessellator.vertexes.data(), sizeof(glm::vec3) * tessellator.vertexes.size(), GL_FLOAT, 3, GL_STATIC_DRAW);
		this->indicesBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, tessellator.indices.data(), sizeof(GLuint) * tessellator.indices.size(), GL_UNSIGNED_INT, 1, GL_STATIC_DRAW);
		this->colorsBuffer.setData(GL_ARRAY_BUFFER, tessellator.colors.data(), sizeof(glm::vec3) * tessellator.colors.size(), GL_FLOAT, 3, GL_STATIC_DRAW);
		this->vertexNumber = tessellator.indices.size();
		this->created = nanotime;
	}

	void DroppedBlock::draw()
	{
		Main::getTerrain()->bind();
		Main::getDroppedShader().program->use();
		glm::mat4 model(1);
		model = glm::translate(model, this->pos);
		model = glm::translate(model, glm::vec3(0, std::sin((nanotime + this->created) / 500000000.) * .1, 0));
		model = glm::scale(model, glm::vec3(.2, .2, .2));
		model = glm::rotate(model, glm::vec2((nanotime + this->created) / 1000000000., 0).x, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-.5, -.5, -.5));
		glm::mat4 mvp = this->world.getPlayer().getViewProjMat() * model;
		Main::getDroppedShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		Main::getDroppedShader().mLocation->setMat4f(model);
		Main::getDroppedShader().mvpLocation->setMat4f(mvp);
		Main::getDroppedShader().texCoordsLocation->setVertexBuffer(this->texCoordsBuffer);
		Main::getDroppedShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		Main::getDroppedShader().colorsLocation->setVertexBuffer(this->colorsBuffer);
		this->indicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glEnable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, this->vertexNumber, GL_UNSIGNED_INT, NULL);
	}

}
