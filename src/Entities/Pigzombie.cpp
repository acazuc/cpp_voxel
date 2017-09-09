#include "Pigzombie.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Pigzombie::head;
	BodyPart *Pigzombie::headH;
	BodyPart *Pigzombie::body;
	BodyPart *Pigzombie::armL;
	BodyPart *Pigzombie::armR;
	BodyPart *Pigzombie::legL;
	BodyPart *Pigzombie::legR;

	void Pigzombie::init()
	{
		head = new BodyPart(glm::vec3(-4, 0, -4), glm::vec3(8, 8, 8), glm::vec2(0, 0));
		head->setPos(glm::vec3(0, 9, 0));
		headH = new BodyPart(glm::vec3(-4, 0, -4), glm::vec3(8, 8, 8), glm::vec2(32, 0));
		headH->setPos(glm::vec3(0, 6.4, 0));
		body = new BodyPart(glm::vec3(-4, -3, -2), glm::vec3(8, 12, 4), glm::vec2(16, 16));
		armL = new BodyPart(glm::vec3(0, -12, -2), glm::vec3(4, 12, 4), glm::vec2(40, 16));
		armL->setPos(glm::vec3(4, 9, 0));
		armR = new BodyPart(glm::vec3(-4, -12, -2), glm::vec3(4, 12, 4), glm::vec2(40, 16));
		armR->setPos(glm::vec3(-4, 9, 0));
		legL = new BodyPart(glm::vec3(-2, -12, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legL->setPos(glm::vec3(-2, -3, 0));
		legR = new BodyPart(glm::vec3(-2, -12, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legR->setPos(glm::vec3(2, -3, 0));
	}

	Pigzombie::Pigzombie(World &world)
	: Entity(world)
	{
		setSize(.6, 1.8, .6);
	}

	void Pigzombie::draw()
	{
		Main::getEntityShader().program->use();
		EntitiesManager::getPigzombie()->bind();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(this->pos.x, this->pos.y, this->pos.z));
		model = glm::rotate(model, this->rot.z, glm::vec3(0, 0, 1));
		model = glm::rotate(model, this->rot.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, this->rot.x, glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(.06, .06, .06));
		Main::getEntityShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		glm::vec4 col(1, 1, 1, 1);
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
		{
			glm::mat4 model2(model);
			model2 = glm::scale(model2, glm::vec3(10. / 8, 10. / 8, 10. / 8));
			headH->draw(&this->world, model2);
		}
	}

}
