#include "Entity.h"
#include "TickManager.h"
#include "World.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	Entity::Entity(World &world)
	: world(world)
	, aabb(0, 0, 0, 0, 0, 0)
	, gravity(.08)
	, isOnFloor(false)
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
		move(this->posDst.x, this->posDst.y, this->posDst.z);
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
		setPos(pos.x, pos.y, pos.z);
	}

	void Entity::setPos(float x, float y, float z)
	{
		this->posOrg.x = x;
		this->posOrg.y = y;
		this->posOrg.z = z;
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		float w = this->size.x / 2;
		float h = this->size.y / 2;
		float d = this->size.z / 2;
		this->aabb.set(x - w, y - h, z - d, x + w, y + h, z + d);
	}

	void Entity::move(float x, float y, float z)
	{
		float xOrg = x;
		float yOrg = y;
		float zOrg = z;
		AABB newAabb = this->aabb.expand(x, y, z);
		std::vector<AABB> aabbs;
		this->world.getAABBs(newAabb, aabbs);
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			y = aabbs[i].collideY(this->aabb, y);
		this->aabb.move(0, y, 0);
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			x = aabbs[i].collideX(this->aabb, x);
		this->aabb.move(x, 0, 0);
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			z = aabbs[i].collideZ(this->aabb, z);
		this->aabb.move(0, 0, z);
		this->pos.x = (this->aabb.getX0() + this->aabb.getX1()) / 2;
		this->pos.y = (this->aabb.getY0() + this->aabb.getY1()) / 2;
		this->pos.z = (this->aabb.getZ0() + this->aabb.getZ1()) / 2;
		if (x != xOrg)
			this->posDst.x = 0;
		if (y != yOrg)
			this->posDst.y = 0;
		if (z != zOrg)
			this->posDst.z = 0;
		this->isOnFloor = yOrg < 0 && y != yOrg;
	}

	void Entity::setSize(float width, float height, float depth)
	{
		this->size.x = width;
		this->size.y = height;
		this->size.z = depth;
		float w = this->size.x / 2;
		float h = this->size.y / 2;
		float d = this->size.z / 2;
		this->aabb.setX0(this->pos.x - w);
		this->aabb.setX1(this->pos.x + w);
		this->aabb.setY0(this->pos.y - h);
		this->aabb.setY1(this->pos.y + h);
		this->aabb.setZ0(this->pos.z - d);
		this->aabb.setZ0(this->pos.z + d);
	}

	glm::vec3 Entity::getRealPos()
	{
		return (this->posOrg + ((this->pos - this->posOrg) * TickManager::getDelta()));
	}

}
