#include "Skeleton.h"
#include "World/World.h"
#include "Main.h"

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
		head = new BodyPart(Vec3(-4, 0, -4), Vec3(8, 8, 8), Vec2(0, 0));
		head->setPos(Vec3(0, 7.4, 0));
		body = new BodyPart(Vec3(-4, -3, -2), Vec3(8, 12, 4), Vec2(16, 16));
		body->setPos(Vec3(0, -1.6, 0));
		armL = new BodyPart(Vec3(0, -11, -1), Vec3(2, 12, 2), Vec2(40, 16));
		armL->setPos(Vec3(4, 6.4, 0));
		armR = new BodyPart(Vec3(-2, -11, -1), Vec3(2, 12, 2), Vec2(40, 16));
		armR->setPos(Vec3(-4, 6.4, 0));
		legL = new BodyPart(Vec3(-1, -12, -1), Vec3(2, 12, 2), Vec2(0, 16));
		legL->setPos(Vec3(-2, -4.6, 0));
		legR = new BodyPart(Vec3(-1, -12, -1), Vec3(2, 12, 2), Vec2(0, 16));
		legR->setPos(Vec3(2, -4.6, 0));
	}

	void Skeleton::clear()
	{
		delete (head);
		delete (body);
		delete (armL);
		delete (armR);
		delete (legL);
		delete (legR);
	}

	Skeleton::Skeleton(World &world, Chunk *chunk)
	: Entity(world, chunk)
	{
		setSize(Vec3(.6, 1.99, .6));
	}

	void Skeleton::draw()
	{
		EntitiesManager::getSkeleton()->bind();
		Vec3 pos = getRealPos();
		Mat4 model = Mat4::translate(model, pos);
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
