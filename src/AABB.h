#ifndef AABB_H
# define AABB_H

# include <glm/vec3.hpp>

namespace voxel
{

	class AABB
	{

	private:
		glm::vec3 p0;
		glm::vec3 p1;

	public:
		AABB();
		AABB(glm::vec3 p0, glm::vec3 p1);
		void set(glm::vec3 p0, glm::vec3 p1);
		void move(glm::vec3 dst);
		AABB grow(glm::vec3 size);
		AABB expand(glm::vec3 size);
		float collideX(AABB &other, float x);
		float collideY(AABB &other, float y);
		float collideZ(AABB &other, float z);
		bool intersect(AABB &other);
		inline glm::vec3 &getP0() {return (this->p0);};
		inline glm::vec3 &getP1() {return (this->p1);};

	};

}

#endif
