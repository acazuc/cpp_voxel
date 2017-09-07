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
		glm::vec3 pos;
		glm::vec3 rot;
		World &world;
		AABB aabb;
		int64_t fallStarted;
		float gravity;
		float height;
		float width;
		float depth;
		bool isOnFloor;
		bool hasJumped;
		bool flying;

	public:
		Entity(World &world);
		virtual void tick();
		void setPos(float x, float y, float z);
		void move(float x, float y, float z);
		void setHeight(float height);
		void setWidth(float width);
		void setDepth(float depth);
		inline glm::vec3 &getPos() {return (this->pos);};
		inline glm::vec3 &getRot() {return (this->rot);};
		inline AABB &getAABB() {return (this->aabb);};

	};

}

#endif
