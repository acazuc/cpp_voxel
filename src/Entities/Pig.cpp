#include "Pig.h"
#include "World/World.h"
#include "Main.h"

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
		head = new BodyPart(Vec3(-4, 0, -4), Vec3(8, 8, 8), Vec2(0, 0));
		head->setPos(Vec3(0, .6, 10));
		body = new BodyPart(Vec3(-5, -8, -4), Vec3(10, 16, 8), Vec2(28, 8));
		body->setRot(Vec3(M_PI / 2, 0, 0));
		body->setPos(Vec3(0, 2.6, 0));
		legFL = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legFL->setPos(Vec3(-3, -1.4, 5));
		legFR = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legFR->setPos(Vec3(3, -1.4, 5));
		legBL = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legBL->setPos(Vec3(-3, -1.4, -7));
		legBR = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legBR->setPos(Vec3(3, -1.4, -7));
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

	Pig::Pig(World &world, Chunk *chunk)
	: Entity(world, chunk)
	{
		setSize(Vec3(.9, .9, .9));
	}

	void Pig::draw()
	{
		EntitiesManager::getPig()->bind();
		Vec3 pos = getRealPos();
		Mat4 model = Mat4::translate(Mat4(1), pos);
		model = Mat4::rotateZ(model, this->rot.z);
		model = Mat4::rotateY(model, this->rot.y);
		model = Mat4::rotateX(model, this->rot.x);
		model = Mat4::scale(model, Vec3(.06, .06, .06));
		Main::getEntityShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		Vec4 col(1, 1, 1, 1);
		col *= ChunkBlock::getLightValue(this->world.getLight(pos.x, pos.y, pos.z));
		col.a = 1;
		Main::getEntityShader().colorLocation->setVec4f(col);
		head->draw(&this->world, model);
		body->draw(&this->world, model);
		legFL->draw(&this->world, model);
		legFR->draw(&this->world, model);
		legBL->draw(&this->world, model);
		legBR->draw(&this->world, model);
	}

}
