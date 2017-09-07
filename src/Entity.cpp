#include "Entity.h"
#include "World.h"
#include "Main.h"

#define JUMP_FORCE .155
#define GRAVITY 0.55

extern int64_t nanotime;

namespace voxel
{

	Entity::Entity(World &world)
	: world(world)
	, aabb(0, 0, 0, 0, 0, 0)
	, fallStarted(0)
	, gravity(0)
	, height(0)
	, width(0)
	, depth(0)
	, isOnFloor(true)
	, flying(true)
	{
		this->fallStarted = nanotime;
	}

	void Entity::tick()
	{
		if (!this->flying)
		{
			if (this->isOnFloor)
			{
				if (Main::getWindow()->isKeyDown(GLFW_KEY_SPACE))
				{
					this->isOnFloor = false;
					this->fallStarted = nanotime;
					this->gravity = -JUMP_FORCE;
					this->hasJumped = true;
				}
			}
			else
			{
				this->gravity = GRAVITY * ((nanotime - this->fallStarted) / 1000000000.);
				if (this->hasJumped)
					this->gravity -= JUMP_FORCE;
			}
			if (!this->gravity)
				return;
			move(0, -this->gravity, 0);
		}
	}

	void Entity::setPos(float x, float y, float z)
	{
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		float w = this->width / 2;
		float h = this->height / 2;
		float d = this->depth / 2;
		this->aabb.set(x - w, y - h, z - d, x + w, y + h, z + d);
	}

	void Entity::move(float x, float y, float z)
	{
		float yOrg = y;
		bool isOnFloor = this->isOnFloor;
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
		this->isOnFloor = yOrg < 0 && y != yOrg;
		if (this->isOnFloor)
			this->hasJumped = false;
		else if (isOnFloor)
			this->fallStarted = nanotime;
	}

	void Entity::setHeight(float height)
	{
		this->height = height;
		float h = this->height / 2;
		this->aabb.setY0(this->pos.y - h);
		this->aabb.setY1(this->pos.y + h);
	}

	void Entity::setWidth(float width)
	{
		this->width = width;
		float w = this->width / 2;
		this->aabb.setX0(this->pos.x - w);
		this->aabb.setX1(this->pos.x + w);
	}

	void Entity::setDepth(float depth)
	{
		this->depth = depth;
		float d = this->depth / 2;
		this->aabb.setZ0(this->pos.z - d);
		this->aabb.setZ1(this->pos.z + d);
	}

}
