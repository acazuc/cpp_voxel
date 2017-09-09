#include "AABB.h"

namespace voxel
{

	AABB::AABB()
	: p0(0, 0, 0)
	, p1(0, 0, 0)
	{
		//Empty
	}

	AABB::AABB(glm::vec3 p0, glm::vec3 p1)
	: p0(p0)
	, p1(p1)
	{
		//Empty
	}

	void AABB::set(glm::vec3 p0, glm::vec3 p1)
	{
		this->p0 = p0;
		this->p1 = p1;
	}

	void AABB::move(glm::vec3 dst)
	{
		this->p0 += dst;
		this->p1 += dst;
	}

	AABB AABB::grow(glm::vec3 size)
	{
		return (AABB(this->p0 - size, this->p1 + size));
	}

	AABB AABB::expand(glm::vec3 size)
	{
		glm::vec3 p0 = this->p0;
		glm::vec3 p1 = this->p1;
		if (size.x < 0)
			p0.x += size.x;
		else if (size.x > 0)
			p1.x += size.x;
		if (size.y < 0)
			p0.y += size.y;
		else if (size.y > 0)
			p1.y += size.y;
		if (size.z < 0)
			p0.z += size.z;
		else if (size.z > 0)
			p1.z += size.z;
		return (AABB(p0, p1));
	}

	float AABB::collideX(AABB &other, float x)
	{
		if (this->p1.y <= other.p0.y || this->p0.y >= other.p1.y)
			return (x);
		if (this->p1.z <= other.p0.z || this->p0.z >= other.p1.z)
			return (x);
		if (x > 0 && this->p0.x >= other.p1.x)
		{
			float max = this->p0.x - other.p1.x;
			if (max < x)
				return (max);
		}
		else if (x < 0 && this->p1.x <= other.p0.x)
		{
			float max = this->p1.x - other.p0.x;
			if (max > x)
				return (max);
		}
		return (x);
	}

	float AABB::collideY(AABB &other, float y)
	{
		if (this->p1.x <= other.p0.x || this->p0.x >= other.p1.x)
			return (y);
		if (this->p1.z <= other.p0.z || this->p0.z >= other.p1.z)
			return (y);
		if (y > 0 && this->p0.y >= other.p1.y)
		{
			float max = this->p0.y - other.p1.y;
			if (max < y)
				return (max);
		}
		else if (y < 0 && this->p1.y <= other.p0.y)
		{
			float max = this->p1.y - other.p0.y;
			if (max > y)
				return (max);
		}
		return (y);
	}

	float AABB::collideZ(AABB &other, float z)
	{
		if (this->p1.x <= other.p0.x || this->p0.x >= other.p1.x)
			return (z);
		if (this->p1.y <= other.p0.y || this->p0.y >= other.p1.y)
			return (z);
		if (z > 0 && this->p0.z >= other.p1.z)
		{
			float max = this->p0.z - other.p1.z;
			if (max < z)
				return (max);
		}
		else if (z < 0 && this->p1.z <= other.p0.z)
		{
			float max = this->p1.z - other.p0.z;
			if (max > z)
				return (max);
		}
		return (z);
	}

	bool AABB::intersect(AABB &other)
	{
		if (this->p1.x <= other.p0.x || this->p0.x >= other.p1.x)
			return (false);
		if (this->p1.y <= other.p0.y || this->p0.y >= other.p1.y)
			return (false);
		if (this->p1.z <= other.p0.z || this->p0.z >= other.p1.z)
			return (false);
		return (true);
	}

}
