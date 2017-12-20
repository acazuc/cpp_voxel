#ifndef ENTITY_H
# define ENTITY_H

# include "./AABB.h"
# include <glm/vec3.hpp>

namespace voxel
{

	class World;
	class Chunk;

	class Entity
	{

	protected:
		glm::vec3 sliperness;
		glm::vec3 posOrg;
		glm::vec3 posDst;
		glm::vec3 size;
		glm::vec3 pos;
		glm::vec3 rot;
		World &world;
		Chunk *chunk;
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
		void setPos(glm::vec3 pos);
		void move(glm::vec3 dst);
		void jump();
		void setSize(glm::vec3 size);
		glm::vec3 getRealPos();
		inline void setPosDst(glm::vec3 posDst) {this->posDst = posDst;};
		inline glm::vec3 &getPos() {return (this->pos);};
		inline glm::vec3 &getRot() {return (this->rot);};
		inline World &getWorld() {return (this->world);};
		inline AABB &getAABB() {return (this->aabb);};
		inline bool isDeleted() {return (this->deleted);};
		inline bool isInWater() {return (this->inWater);};

	};

}

#endif
