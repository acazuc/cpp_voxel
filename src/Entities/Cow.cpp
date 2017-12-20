#include "Cow.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Cow::head;
	BodyPart *Cow::body;
	BodyPart *Cow::hornL;
	BodyPart *Cow::hornR;
	BodyPart *Cow::legFL;
	BodyPart *Cow::legFR;
	BodyPart *Cow::legBL;
	BodyPart *Cow::legBR;

	void Cow::init()
	{
		head = new BodyPart(glm::vec3(-4, -4, -3), glm::vec3(8, 8, 6), glm::vec2(0, 0));
		head->setPos(glm::vec3(0, 8.4, 12));
		body = new BodyPart(glm::vec3(-6, -9, -5), glm::vec3(12, 18, 10), glm::vec2(18, 4));
		body->setRot(glm::vec3(M_PI / 2, 0, 0));
		body->setPos(glm::vec3(0, 5.4, 0));
		legFL = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legFL->setPos(glm::vec3(-4, -5.6, 6));
		legFR = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legFR->setPos(glm::vec3(4, -5.6, 6));
		legBL = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legBL->setPos(glm::vec3(-4, -5.6, -7));
		legBR = new BodyPart(glm::vec3(-2, -6, -2), glm::vec3(4, 12, 4), glm::vec2(0, 16));
		legBR->setPos(glm::vec3(4, -5.6, -7));
	}

	void Cow::clear()
	{
		delete (head);
		delete (body);
		delete (legFL);
		delete (legFR);
		delete (legBL);
		delete (legBR);
	}

	Cow::Cow(World &world, Chunk *chunk)
	: Entity(world, chunk)
	{
		setSize(glm::vec3(.9, 1.4, .9));
	}

	void Cow::draw()
	{
		glm::vec3 pos = getRealPos();
		EntitiesManager::getCow()->bind();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		model = glm::rotate(model, this->rot.z, glm::vec3(0, 0, 1));
		model = glm::rotate(model, this->rot.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, this->rot.x, glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(.06, .06, .06));
		Main::getEntityShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		glm::vec4 col(1, 1, 1, 1);
		col *= ChunkBlock::getLightValue(this->world.getLight(this->pos.x, this->pos.y, this->pos.z));
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
