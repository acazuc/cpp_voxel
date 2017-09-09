#ifndef ENTITY_H
# define ENTITY_H

# include "./AABB.h"
# include <glm/vec3.hpp>

namespace voxel
{

	class World;

	class Entity
	{

	protected:
		glm::vec3 posOrg;
		glm::vec3 posDst;
		glm::vec3 size;
		glm::vec3 pos;
		glm::vec3 rot;
		World &world;
		AABB aabb;
		bool isOnFloor;
		bool flying;

	public:
		Entity(World &world);
		virtual ~Entity();
		virtual void tick();
		virtual void draw();
		void setPos(glm::vec3 pos);
		void setPos(float x, float y, float z);
		void move(float x, float y, float z);
		void jump();
		void setSize(float x, float y, float z);
		glm::vec3 getRealPos();
		inline glm::vec3 &getPos() {return (this->pos);};
		inline glm::vec3 &getRot() {return (this->rot);};
		inline AABB &getAABB() {return (this->aabb);};

	};

}

#endif
