#ifndef AABB_H
# define AABB_H

# include <librender/Vec/Vec3.h>

using librender::Vec3;

namespace voxel
{

	class AABB
	{

	private:
		Vec3 p0;
		Vec3 p1;

	public:
		AABB();
		AABB(Vec3 p0, Vec3 p1);
		void set(Vec3 p0, Vec3 p1);
		void move(Vec3 dst);
		AABB grow(Vec3 size);
		AABB expand(Vec3 size);
		float collideX(AABB &other, float x);
		float collideY(AABB &other, float y);
		float collideZ(AABB &other, float z);
		bool intersect(AABB &other);
		bool intersect(Vec3 pos, Vec3 dir, float &t);
		inline Vec3 &getP0() {return this->p0;};
		inline Vec3 &getP1() {return this->p1;};

	};

}

#endif
