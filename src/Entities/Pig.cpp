#include "Pig.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Pig::head;
	BodyPart *Pig::body;
	BodyPart *Pig::legFL;
	BodyPart *Pig::legFR;
	BodyPart *Pig::legBL;
	BodyPart *Pig::legBR;

	void Pig::init()
	{
		head = new BodyPart(glm::vec3(-4, 0, -4), glm::vec3(8, 8, 8), glm::vec2(0, 0));
		head->setPos(glm::vec3(0, .6, 10));
		body = new BodyPart(glm::vec3(-5, -8, -4), glm::vec3(10, 16, 8), glm::vec2(28, 8));
		body->setRot(glm::vec3(M_PI / 2, 0, 0));
		body->setPos(glm::vec3(0, 2.6, 0));
		legFL = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 6, 4), glm::vec2(0, 16));
		legFL->setPos(glm::vec3(-3, -1.4, 5));
		legFR = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 6, 4), glm::vec2(0, 16));
		legFR->setPos(glm::vec3(3, -1.4, 5));
		legBL = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 6, 4), glm::vec2(0, 16));
		legBL->setPos(glm::vec3(-3, -1.4, -7));
		legBR = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 6, 4), glm::vec2(0, 16));
		legBR->setPos(glm::vec3(3, -1.4, -7));
	}

	void Pig::clear()
	{
		delete (head);
		delete (body);
		delete (legFL);
		delete (legFR);
		delete (legBL);
		delete (legBR);
	}

	Pig::Pig(World &world)
	: Entity(world, NULL)
	{
		setSize(glm::vec3(.9, .9, .9));
	}

	void Pig::draw()
	{
		glm::vec3 pos = getRealPos();
		EntitiesManager::getPig()->bind();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		model = glm::rotate(model, this->rot.z, glm::vec3(0, 0, 1));
		model = glm::rotate(model, this->rot.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, this->rot.x, glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(.06, .06, .06));
		Main::getEntityShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		glm::vec4 col(1, 1, 1, 1);
		col *= ChunkBlock::getLightValue(this->world.getLight(this->pos));
		col.w = 1;
		Main::getEntityShader().colorLocation->setVec4f(col);
		head->draw(&this->world, model);
		body->draw(&this->world, model);
		legFL->draw(&this->world, model);
		legFR->draw(&this->world, model);
		legBL->draw(&this->world, model);
		legBR->draw(&this->world, model);
	}

}
