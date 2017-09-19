#include "Human.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Human::head;
	BodyPart *Human::body;
	BodyPart *Human::armL;
	BodyPart *Human::armR;
	BodyPart *Human::legL;
	BodyPart *Human::legR;

	void Human::init()
	{
		head = new BodyPart(glm::vec3(-4, 0, -4), glm::vec3(8, 8, 8), glm::vec2(0, 0));
		head->setPos(glm::vec3(0, 9, 0));
		body = new BodyPart(glm::vec3(-4, -3, -2), glm::vec3(8, 12, 4), glm::vec2(16, 16));
		armL = new BodyPart(glm::vec3(0, -10, -2), glm::vec3(4, 12, 4), glm::vec2(40, 16));
		armL->setPos(glm::vec3(4, 7, 0));
		armR = new BodyPart(glm::vec3(-4, -10, -2), glm::vec3(4, 12, 4), glm::vec2(40, 16));
		armR->setPos(glm::vec3(-4, 7, 0));
		legL = new BodyPart(glm::vec3(-2, -12, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legL->setPos(glm::vec3(-2, -3, 0));
		legR = new BodyPart(glm::vec3(-2, -12, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legR->setPos(glm::vec3(2, -3, 0));
	}

	Human::Human(World &world)
	: Entity(world)
	{
		setSize(glm::vec3(.6, 1.8, .6));
	}

	void Human::draw()
	{
		glm::vec3 pos = getRealPos();
		EntitiesManager::getCharacter()->bind();
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
		float time = nanotime / 1000000000.;
		armL->setRotZ(std::sin(time * 1) * .05 + .05);
		armR->setRotZ(-std::sin(time * 1) * .05 - .05);
		head->draw(&this->world, model);
		body->draw(&this->world, model);
		armL->draw(&this->world, model);
		armR->draw(&this->world, model);
		legL->draw(&this->world, model);
		legR->draw(&this->world, model);
	}

}
