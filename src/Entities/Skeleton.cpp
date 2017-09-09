#include "Skeleton.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Skeleton::head;
	BodyPart *Skeleton::body;
	BodyPart *Skeleton::armL;
	BodyPart *Skeleton::armR;
	BodyPart *Skeleton::legL;
	BodyPart *Skeleton::legR;

	void Skeleton::init()
	{
		head = new BodyPart(glm::vec3(-4, 0, -4), glm::vec3(8, 8, 8), glm::vec2(0, 0));
		head->setPos(glm::vec3(0, 7.4, 0));
		body = new BodyPart(glm::vec3(-4, -3, -2), glm::vec3(8, 12, 4), glm::vec2(16, 16));
		body->setPos(glm::vec3(0, -1.6, 0));
		armL = new BodyPart(glm::vec3(0, -12, -1), glm::vec3(2, 12, 2), glm::vec2(40, 16));
		armL->setPos(glm::vec3(4, 7.4, 0));
		armR = new BodyPart(glm::vec3(-2, -12, -1), glm::vec3(2, 12, 2), glm::vec2(40, 16));
		armR->setPos(glm::vec3(-4, 7.4, 0));
		legL = new BodyPart(glm::vec3(-1, -12, -1), glm::vec3(2, 12, 2), glm::vec2(0, 16));
		legL->setPos(glm::vec3(-2, -4.6, 0));
		legR = new BodyPart(glm::vec3(-1, -12, -1), glm::vec3(2, 12, 2), glm::vec2(0, 16));
		legR->setPos(glm::vec3(2, -4.6, 0));
	}

	Skeleton::Skeleton(World &world)
	: Entity(world)
	{
		setSize(.6, 1.99, .6);
	}

	void Skeleton::draw()
	{
		Main::getEntityShader().program->use();
		EntitiesManager::getSkeleton()->bind();
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
	}

}
