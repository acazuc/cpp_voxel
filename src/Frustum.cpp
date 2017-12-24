#include "Frustum.h"
#include "World/World.h"

namespace voxel
{

	void Frustum::update(Mat4 &vp)
	{
		const float *clip = reinterpret_cast<float*>(&vp[0][0]);
		this->data[0][0] = clip[3] - clip[0];
		this->data[0][1] = clip[7] - clip[4];
		this->data[0][2] = clip[11] - clip[8];
		this->data[0][3] = clip[15] - clip[12];
		this->data[0].normalize();
		this->data[1][0] = clip[3] + clip[0];
		this->data[1][1] = clip[7] + clip[4];
		this->data[1][2] = clip[11] + clip[8];
		this->data[1][3] = clip[15] + clip[12];
		this->data[1].normalize();
		this->data[2][0] = clip[3] + clip[1];
		this->data[2][1] = clip[7] + clip[5];
		this->data[2][2] = clip[11] + clip[9];
		this->data[2][3] = clip[15] + clip[13];
		this->data[2].normalize();
		this->data[3][0] = clip[3] - clip[1];
		this->data[3][1] = clip[7] - clip[5];
		this->data[3][2] = clip[11] - clip[9];
		this->data[3][3] = clip[15] - clip[13];
		this->data[3].normalize();
		this->data[4][0] = clip[3] - clip[2];
		this->data[4][1] = clip[7] - clip[6];
		this->data[4][2] = clip[11] - clip[10];
		this->data[4][3] = clip[15] - clip[14];
		this->data[4].normalize();
		this->data[5][0] = clip[3] + clip[2];
		this->data[5][1] = clip[7] + clip[6];
		this->data[5][2] = clip[11] + clip[10];
		this->data[5][3] = clip[15] + clip[14];
		this->data[5].normalize();
	}

	bool Frustum::check(AABB &aabb)
	{
		for (uint8_t i = 0; i < 6; ++i)
		{
			Vec4 data(this->data[i]);
			if (data.x * aabb.getP0().x + data.y * aabb.getP0().y + data.z * aabb.getP0().z + data.w > 0)
				continue;
			if (data.x * aabb.getP1().x + data.y * aabb.getP0().y + data.z * aabb.getP0().z + data.w > 0)
				continue;
			if (data.x * aabb.getP0().x + data.y * aabb.getP1().y + data.z * aabb.getP0().z + data.w > 0)
				continue;
			if (data.x * aabb.getP1().x + data.y * aabb.getP1().y + data.z * aabb.getP0().z + data.w > 0)
				continue;
			if (data.x * aabb.getP0().x + data.y * aabb.getP0().y + data.z * aabb.getP1().z + data.w > 0)
				continue;
			if (data.x * aabb.getP1().x + data.y * aabb.getP0().y + data.z * aabb.getP1().z + data.w > 0)
				continue;
			if (data.x * aabb.getP0().x + data.y * aabb.getP1().y + data.z * aabb.getP1().z + data.w > 0)
				continue;
			if (data.x * aabb.getP1().x + data.y * aabb.getP1().y + data.z * aabb.getP1().z + data.w > 0)
				continue;
			return (false);
		}
		return (true);
	}

}
