#include "Slime.h"
#include "World/World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	BodyPart *Slime::body;

	void Slime::init()
	{
		body = new BodyPart(Vec3(-4, -4, -4), Vec3(8, 8, 8), Vec2(0, 0));
		body->setPos(Vec3(0, -4.4, 0));
	}

	void Slime::clear()
	{
		delete (body);
	}

	Slime::Slime(World &world, Chunk *chunk)
	: Entity(world, chunk)
	{
		setSize(Vec3(1.01, 1.01, 1.01));
	}

	void Slime::draw()
	{
		EntitiesManager::getSlime()->bind();
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
		body->draw(&this->world, model);
	}

}
