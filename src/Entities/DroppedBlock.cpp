#include "DroppedBlock.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	DroppedBlock::DroppedBlock(World &world, uint8_t type)
	: Entity(world)
	, world(world)
	, created(nanotime)
	, number(1)
	, type(type)
	{
		setSize(glm::vec3(.25, .25, .25));
	}

	void DroppedBlock::draw()
	{
		Main::getTerrain()->bind();
		Main::getDroppedShader().program->use();
		Main::getDroppedShader().texCoordsLocation->setVertexBuffer(this->texCoordsBuffer);
		Main::getDroppedShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		glm::vec4 color(1);
		Main::getDroppedShader().colorLocation->setVec4f(color);
		this->indicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, this->vertexNumber, GL_UNSIGNED_INT, NULL);
	}

}
