#include "Creeper.h"
#include "World/World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Creeper::head;
	BodyPart *Creeper::body;
	BodyPart *Creeper::legFL;
	BodyPart *Creeper::legFR;
	BodyPart *Creeper::legBL;
	BodyPart *Creeper::legBR;

	void Creeper::init()
	{
		head = new BodyPart(Vec3(-4, 0, -4), Vec3(8, 8, 8), Vec2(0, 0));
		head->setPos(Vec3(0, 4, 0));
		body = new BodyPart(Vec3(-4, -3, -2), Vec3(8, 12, 4), Vec2(16, 16));
		body->setPos(Vec3(0, -5, 0));
		legFL = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legFL->setPos(Vec3(-2, -8, 4));
		legFR = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legFR->setPos(Vec3(2, -8, 4));
		legBL = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legBL->setPos(Vec3(-2, -8, -4));
		legBR = new BodyPart(Vec3(-2, -6, -2), Vec3(4, 6, 4), Vec2(0, 16));
		legBR->setPos(Vec3(2, -8, -4));
	}

	void Creeper::clear()
	{
		delete (head);
		delete (body);
		delete (legFL);
		delete (legFR);
		delete (legBL);
		delete (legBR);
	}

	Creeper::Creeper(World &world, Chunk *chunk)
	: Entity(world, chunk)
	{
		setSize(Vec3(.6, 1.7, .6));
	}

	void Creeper::draw()
	{
		EntitiesManager::getCreeper()->bind();
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
