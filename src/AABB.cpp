#include "AABB.h"

namespace voxel
{

	AABB::AABB(float x0, float y0, float z0, float x1, float y1, float z1)
	: x0(x0)
	, y0(y0)
	, z0(z0)
	, x1(x1)
	, y1(y1)
	, z1(z1)
	{
		//Empty
	}

	void AABB::set(float x0, float y0, float z0, float x1, float y1, float z1)
	{
		this->x0 = x0;
		this->y0 = y0;
		this->z0 = z0;
		this->x1 = x1;
		this->y1 = y1;
		this->z1 = z1;
	}

	void AABB::move(float x, float y, float z)
	{
		this->x0 += x;
		this->y0 += y;
		this->z0 += z;
		this->x1 += x;
		this->y1 += y;
		this->z1 += z;
	}

	AABB AABB::grow(float x, float y, float z)
	{
		float x0 = this->x0 - x;
		float y0 = this->y0 - y;
		float z0 = this->z0 - z;
		float x1 = this->x1 + x;
		float y1 = this->y1 + y;
		float z1 = this->z1 + z;
		return (AABB(x0, y0, z0, x1, y1, z1));
	}

	AABB AABB::expand(float x, float y, float z)
	{
		float x0 = this->x0;
		float y0 = this->y0;
		float z0 = this->z0;
		float x1 = this->x1;
		float y1 = this->y1;
		float z1 = this->z1;
		if (x < 0)
			x0 += x;
		else if (x > 0)
			x1 += x;
		if (y < 0)
			y0 += y;
		else if (y > 0)
			y1 += y;
		if (z < 0)
			z0 += z;
		else if (z > 0)
			z1 += z;
		return (AABB(x0,  y0, z0, x1, y1, z1));
	}

	float AABB::collideX(AABB &other, float x)
	{
		if (this->y1 <= other.y0 || this->y0 >= other.y1)
			return (x);
		if (this->z1 <= other.z0 || this->z0 >= other.z1)
			return (x);
		if (x > 0 && this->x0 >= other.x1)
		{
			float max = this->x0 - other.x1;
			if (max < x)
				return (max);
		}
		else if (x < 0 && this->x1 <= other.x0)
		{
			float max = this->x1 - other.x0;
			if (max > x)
				return (max);
		}
		return (x);
	}

	float AABB::collideY(AABB &other, float y)
	{
		if (this->x1 <= other.x0 || this->x0 >= other.x1)
			return (y);
		if (this->z1 <= other.z0 || this->z0 >= other.z1)
			return (y);
		if (y > 0 && this->y0 >= other.y1)
		{
			float max = this->y0 - other.y1;
			if (max < y)
				return (max);
		}
		else if (y < 0 && this->y1 <= other.y0)
		{
			float max = this->y1 - other.y0;
			if (max > y)
				return (max);
		}
		return (y);
	}

	float AABB::collideZ(AABB &other, float z)
	{
		if (this->x1 <= other.x0 || this->x0 >= other.x1)
			return (z);
		if (this->y1 <= other.y0 || this->y0 >= other.y1)
			return (z);
		if (z > 0 && this->z0 >= other.z1)
		{
			float max = this->z0 - other.z1;
			if (max < z)
				return (max);
		}
		else if (z < 0 && this->z1 <= other.z0)
		{
			float max = this->z1 - other.z0;
			if (max > z)
				return (max);
		}
		return (z);
	}

	bool AABB::intersect(AABB &other)
	{
		if (this->x1 <= other.x0 || this->x0 >= other.x1)
			return (false);
		if (this->y1 <= other.y0 || this->y0 >= other.y1)
			return (false);
		if (this->z1 <= other.z0 || this->z0 >= other.z1)
			return (false);
		return (true);
	}

}
