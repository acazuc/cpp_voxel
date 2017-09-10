#include "Entity.h"
#include "World/World.h"
#include "TickManager.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	Entity::Entity(World &world)
	: world(world)
	, aabb()
	, gravity(.08)
	, isOnFloor(false)
	, deleted(false)
	, flying(false)
	{
		//Empty
	}

	Entity::~Entity()
	{
		//Empty
	}

	void Entity::tick()
	{
		this->posOrg = this->pos;
		if (!this->flying)
			this->posDst.y -= this->gravity;
		move(this->posDst);
		this->posDst.x *= .91;
		this->posDst.y *= .98;
		this->posDst.z *= .91;
		if (this->flying || this->isOnFloor)
		{
			this->posDst.x *= .7;
			this->posDst.z *= .7;
		}
		if (this->flying)
			this->posDst.y *= .7;
	}

	void Entity::draw()
	{
		//Empty
	}

	void Entity::jump()
	{
		this->isOnFloor = false;
		this->posDst.y += .5;
	}

	void Entity::setPos(glm::vec3 pos)
	{
		this->posOrg = pos;
		this->pos = pos;
		glm::vec3 semiSize = this->size / 2.f;
		this->aabb.set(pos - semiSize, pos + semiSize);
	}

	void Entity::move(glm::vec3 dir)
	{
		glm::vec3 org = dir;
		AABB newAabb = this->aabb.expand(dir);
		std::vector<AABB> aabbs;
		this->world.getAABBs(newAabb, aabbs);
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			dir.y = aabbs[i].collideY(this->aabb, dir.y);
		this->aabb.move(glm::vec3(0, dir.y, 0));
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			dir.x = aabbs[i].collideX(this->aabb, dir.x);
		this->aabb.move(glm::vec3(dir.x, 0, 0));
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			dir.z = aabbs[i].collideZ(this->aabb, dir.z);
		this->aabb.move(glm::vec3(0, 0, dir.z));
		this->pos = (this->aabb.getP0() + this->aabb.getP1()) / 2.f;
		if (dir.x != org.x)
			this->posDst.x = 0;
		if (dir.y != org.y)
			this->posDst.y = 0;
		if (dir.z != org.z)
			this->posDst.z = 0;
		this->isOnFloor = org.y < 0 && dir.y != org.y;
		if (this->pos.y < -100)
			this->deleted = true;
	}

	void Entity::setSize(glm::vec3 size)
	{
		this->size = size;
		glm::vec3 semiSize = size / 2.f;
		this->aabb.set(this->pos - semiSize, this->pos + semiSize);
	}

	glm::vec3 Entity::getRealPos()
	{
		return (this->posOrg + ((this->pos - this->posOrg) * TickManager::getDelta()));
	}

}
