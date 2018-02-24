#include "Entity.h"
#include "World/World.h"
#include "TickManager.h"
#include "Main.h"

extern int64_t nanotime;

namespace voxel
{

	Entity::Entity(World &world, Chunk *chunk)
	: world(world)
	, chunk(chunk)
	, aabb()
	, gravity(.08)
	, isOnFloor(false)
	, deleted(false)
	, flying(false)
	{
		this->sliperness = Vec3(.91, .98, .91);
	}

	Entity::~Entity()
	{
		//Empty
	}

	void Entity::tick()
	{
		if (this != &this->world.getPlayer() && !this->chunk)
			updateParentChunk();
		this->posOrg = this->pos;
		if (!this->flying)
			this->posDst.y -= this->gravity;
		move(this->posDst);
		this->posDst *= this->sliperness;
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

	void Entity::setPos(Vec3 pos)
	{
		this->posOrg = pos;
		this->pos = pos;
		Vec3 semiSize = this->size / 2.f;
		this->aabb.set(pos - semiSize, pos + semiSize);
		updateParentChunk();
	}

	void Entity::updateGravitySliperness()
	{
		Vec3 pos(this->world.getPlayer().getPos());
		pos.y -= this->size.y / 2 - .4;
		ChunkBlock *block = this->world.getBlock(pos.x, pos.y, pos.z);
		this->inWater = block && (block->getType() == 8 || block->getType() == 9);
		if (this->inWater && !this->flying)
			this->sliperness = Vec3(.8);
		else
			this->sliperness = Vec3(.91, .98, .91);
	}

	void Entity::move(Vec3 dir)
	{
		updateGravitySliperness();
		Vec3 org = dir;
		AABB newAabb = this->aabb.expand(dir);
		std::vector<AABB> aabbs;
		this->world.getAABBs(newAabb, aabbs);
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			dir.y = aabbs[i].collideY(this->aabb, dir.y);
		this->aabb.move(Vec3(0, dir.y, 0));
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			dir.x = aabbs[i].collideX(this->aabb, dir.x);
		this->aabb.move(Vec3(dir.x, 0, 0));
		for (uint32_t i = 0; i < aabbs.size(); ++i)
			dir.z = aabbs[i].collideZ(this->aabb, dir.z);
		this->aabb.move(Vec3(0, 0, dir.z));
		this->pos = (this->aabb.getP0() + this->aabb.getP1()) / 2.f;
		if (dir.x != org.x)
			this->posDst.x = 0;
		if (dir.y != org.y)
			this->posDst.y = 0;
		if (dir.z != org.z)
			this->posDst.z = 0;
		if (!this->flying && this->inWater && (dir.x != org.x || dir.z != org.z))
		{
			AABB tmp = this->aabb;
			tmp.move(Vec3(this->posDst.x, this->posDst.y + .6f, this->posDst.z));
			std::vector<AABB> tmp2;
			this->world.getAABBs(tmp, tmp2);
			if (tmp2.size() == 0)
				this->posDst.y = .3;
		}
		this->isOnFloor = org.y < 0 && dir.y != org.y;
		if (this->pos.y < -100)
		{
			this->deleted = true;
			return;
		}
		if (dir.x != org.x || dir.y != org.y || dir.z != org.z)
			updateParentChunk();
	}

	void Entity::setSize(Vec3 size)
	{
		this->size = size;
		Vec3 semiSize = size / 2.f;
		Vec3 org = this->pos - semiSize;
		//org.y = this->pos.y;
		Vec3 dst = this->pos + semiSize;
		//dst.y = this->pos.y + size.y;
		this->aabb.set(org, dst);
	}

	Vec3 Entity::getRealPos()
	{
		return (this->posOrg + ((this->pos - this->posOrg) * TickManager::getDelta()));
	}

	void Entity::updateParentChunk()
	{
		if (this->deleted)
			return;
		int32_t chunkX = World::getChunkCoord(this->pos.x);
		int32_t chunkZ = World::getChunkCoord(this->pos.z);
		if (!this->chunk || (this->chunk->getX() != chunkX && this->chunk->getZ() != chunkZ))
		{
			Chunk *chunk = NULL;
			if (!(chunk = this->world.getChunk(chunkX, chunkZ)))
			{
				this->deleted = true;
				return;
			}
			if (this->chunk)
				this->chunk->getEntitiesManager().removeEntity(this);
			this->chunk = chunk;
			this->chunk->getEntitiesManager().addEntity(this);
		}
	}

}
