#include "Slime.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Slime::body;

	void Slime::init()
	{
		body = new BodyPart(glm::vec3(-4, -4, -4), glm::vec3(8, 8, 8), glm::vec2(0, 0));
		body->setPos(glm::vec3(0, -4.4, 0));
	}

	Slime::Slime(World &world)
	: Entity(world)
	{
		setSize(glm::vec3(1.01, 1.01, 1.01));
	}

	void Slime::draw()
	{
		glm::vec3 pos = getRealPos();
		EntitiesManager::getSlime()->bind();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		model = glm::rotate(model, this->rot.z, glm::vec3(0, 0, 1));
		model = glm::rotate(model, this->rot.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, this->rot.x, glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(.06, .06, .06));
		Main::getEntityShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		glm::vec4 col(1, 1, 1, 1);
		col *= ChunkBlock::getLightValue(15);
		col.w = 1;
		Main::getEntityShader().colorLocation->setVec4f(col);
		body->draw(&this->world, model);
	}

}
