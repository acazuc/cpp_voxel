#ifndef ENTITY_H
# define ENTITY_H

# include "./AABB.h"
# include <glm/vec3.hpp>

# define JUMP_FORCE .24
# define GRAVITY 0.98

namespace voxel
{

	class World;

	class Entity
	{

	protected:
		glm::vec3 size;
		glm::vec3 pos;
		glm::vec3 rot;
		World &world;
		AABB aabb;
		int64_t fallStarted;
		float gravity;
		bool isOnFloor;
		bool hasJumped;
		bool flying;

	public:
		Entity(World &world);
		virtual void tick();
		void setPos(float x, float y, float z);
		void move(float x, float y, float z);
		void jump();
		void setSize(float x, float y, float z);
		inline glm::vec3 &getPos() {return (this->pos);};
		inline glm::vec3 &getRot() {return (this->rot);};
		inline AABB &getAABB() {return (this->aabb);};

	};

}

#endif
