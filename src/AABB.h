#ifndef AABB_H
# define AABB_H

namespace voxel
{

	class AABB
	{

	private:
		float x0;
		float y0;
		float z0;
		float x1;
		float y1;
		float z1;

	public:
		AABB(float x0, float y0, float z0, float x1, float y1, float z1);
		void set(float x0, float y0, float z0, float x1, float y1, float z1);
		void move(float x, float y, float z);
		AABB grow(float x, float y, float z);
		AABB expand(float x, float y, float z);
		float collideX(AABB &other, float x);
		float collideY(AABB &other, float y);
		float collideZ(AABB &other, float z);
		bool intersect(AABB &other);
		inline void setX0(float x0) {this->x0 = x0;};
		inline float getX0() {return (this->x0);};
		inline void setY0(float y0) {this->y0 = y0;};
		inline float getY0() {return (this->y0);};
		inline void setZ0(float z0) {this->z0 = z0;};
		inline float getZ0() {return (this->z0);};
		inline void setX1(float x1) {this->x1 = x1;};
		inline float getX1() {return (this->x1);};
		inline void setY1(float y1) {this->y1 = y1;};
		inline float getY1() {return (this->y1);};
		inline void setZ1(float z1) {this->z1 = z1;};
		inline float getZ1() {return (this->z1);};

	};

}

#endif
