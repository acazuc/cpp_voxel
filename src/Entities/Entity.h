#ifndef ENTITY_H
# define ENTITY_H

# include "./AABB.h"
# include <librender/Vec/Vec3.h>

using librender::Vec3;

namespace voxel
{

	class World;
	class Chunk;

	class Entity
	{

	protected:
		World &world;
		Chunk *chunk;
		Vec3 sliperness;
		Vec3 posOrg;
		Vec3 posDst;
		Vec3 size;
		Vec3 pos;
		Vec3 rot;
		AABB aabb;
		float gravity;
		bool isOnFloor;
		bool deleted;
		bool inWater;
		bool flying;
		void updateGravitySliperness();
		virtual void updateParentChunk();

	public:
		Entity(World &world, Chunk *chunk);
		virtual ~Entity();
		virtual void tick();
		virtual void draw();
		void setPos(Vec3 pos);
		void move(Vec3 dst);
		void jump();
		void setSize(Vec3 size);
		Vec3 getRealPos();
		inline void setPosDst(Vec3 posDst) {this->posDst = posDst;};
		inline Vec3 &getPos() {return (this->pos);};
		inline Vec3 &getRot() {return (this->rot);};
		inline World &getWorld() {return (this->world);};
		inline AABB &getAABB() {return (this->aabb);};
		inline bool isDeleted() {return (this->deleted);};
		inline bool isInWater() {return (this->inWater);};

	};

}

#endif
