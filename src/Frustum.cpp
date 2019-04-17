#include "Frustum.h"
#include "World/World.h"

namespace voxel
{

	void Frustum::update(Mat4 &vp)
	{
		const float *clip = reinterpret_cast<float*>(&vp[0][0]);
		this->planes.resize(6);
		this->planes[0] = normalize(Vec4(clip[3] - clip[0], clip[7] - clip[4], clip[11] - clip[8], clip[15] - clip[12]));
		this->planes[1] = normalize(Vec4(clip[3] + clip[0], clip[7] + clip[4], clip[11] + clip[8], clip[15] + clip[12]));
		this->planes[2] = normalize(Vec4(clip[3] + clip[1], clip[7] + clip[5], clip[11] + clip[9], clip[15] + clip[13]));
		this->planes[3] = normalize(Vec4(clip[3] - clip[1], clip[7] - clip[5], clip[11] - clip[9], clip[15] - clip[13]));
		this->planes[4] = normalize(Vec4(clip[3] - clip[2], clip[7] - clip[6], clip[11] - clip[10], clip[15] - clip[14]));
		this->planes[5] = normalize(Vec4(clip[3] + clip[2], clip[7] + clip[6], clip[11] + clip[10], clip[15] + clip[14]));
	}

	bool Frustum::check(AABB &aabb)
	{
		Vec4 points[8] = {
			Vec4(aabb.getP0().x, aabb.getP0().y, aabb.getP0().z, 1),
			Vec4(aabb.getP1().x, aabb.getP0().y, aabb.getP0().z, 1),
			Vec4(aabb.getP0().x, aabb.getP1().y, aabb.getP0().z, 1),
			Vec4(aabb.getP1().x, aabb.getP1().y, aabb.getP0().z, 1),
			Vec4(aabb.getP0().x, aabb.getP0().y, aabb.getP1().z, 1),
			Vec4(aabb.getP1().x, aabb.getP0().y, aabb.getP1().z, 1),
			Vec4(aabb.getP0().x, aabb.getP1().y, aabb.getP1().z, 1),
			Vec4(aabb.getP1().x, aabb.getP1().y, aabb.getP1().z, 1)};
		for (size_t i = 0; i < this->planes.size(); ++i)
		{
			Vec4 data(this->planes[i]);
			if (dot(data, points[0]) > 0
			 || dot(data, points[1]) > 0
			 || dot(data, points[2]) > 0
			 || dot(data, points[3]) > 0
			 || dot(data, points[4]) > 0
			 || dot(data, points[5]) > 0
			 || dot(data, points[6]) > 0
			 || dot(data, points[7]) > 0)
				continue;
			return false;
		}
		return true;
	}

}
