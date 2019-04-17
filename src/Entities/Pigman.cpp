#include "Pigman.h"
#include "World/World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Pigman::head;
	BodyPart *Pigman::body;
	BodyPart *Pigman::armL;
	BodyPart *Pigman::armR;
	BodyPart *Pigman::legL;
	BodyPart *Pigman::legR;

	void Pigman::init()
	{
		head = new BodyPart(Vec3(-4, 0, -4), Vec3(8, 8, 8), Vec2(0, 0));
		head->setPos(Vec3(0, 9, 0));
		body = new BodyPart(Vec3(-4, -3, -2), Vec3(8, 12, 4), Vec2(16, 16));
		armL = new BodyPart(Vec3(0, -10, -2), Vec3(4, 12, 4), Vec2(40, 16));
		armL->setPos(Vec3(4, 7, 0));
		armR = new BodyPart(Vec3(-4, -10, -2), Vec3(4, 12, 4), Vec2(40, 16));
		armR->setPos(Vec3(-4, 7, 0));
		legL = new BodyPart(Vec3(-2, -12, -2), Vec3(4, 12, 4), Vec2(0, 16));
		legL->setPos(Vec3(-2, -3, 0));
		legR = new BodyPart(Vec3(-2, -12, -2), Vec3(4, 12, 4), Vec2(0, 16));
		legR->setPos(Vec3(2, -3, 0));
	}

	void Pigman::clear()
	{
		delete (head);
		delete (body);
		delete (armL);
		delete (armR);
		delete (legL);
		delete (legR);
	}

	Pigman::Pigman(World &world, Chunk *chunk)
	: Entity(world, chunk)
	{
		setSize(Vec3(.6, 1.8, .6));
	}

	void Pigman::draw()
	{
		EntitiesManager::getPigman()->bind();
		Vec3 pos = getRealPos();
		Mat4 model = Mat4::translate(Mat4(1), pos);
		model = Mat4::rotateZ(model, this->rot.z);
		model = Mat4::rotateY(model, this->rot.y);
		model = Mat4::rotateX(model, this->rot.x);
		model = Mat4::scale(model, Vec3(.06, .06, .06));
		Main::getEntityShader().vLocation.setMat4f(this->world.getPlayer().getViewMat());
		Vec4 col(1, 1, 1, 1);
		col *= ChunkBlock::getLightValue(this->world.getLight(pos.x, pos.y, pos.z));
		col.a = 1;
		Main::getEntityShader().colorLocation.setVec4f(col);
		float time = nanotime / 1000000000.;
		armL->setRot(Vec3(0, 0, std::sin(time * 1) * .05 + .05));
		armR->setRot(Vec3(0, 0, -std::sin(time * 1) * .05 - .05));
		head->draw(&this->world, model);
		body->draw(&this->world, model);
		armL->draw(&this->world, model);
		armR->draw(&this->world, model);
		legL->draw(&this->world, model);
		legR->draw(&this->world, model);
	}

}
