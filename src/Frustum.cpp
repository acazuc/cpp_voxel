#include "Frustum.h"
#include "World.h"
#include <glm/gtc/type_ptr.hpp>

namespace voxel
{

	Frustum::Frustum(World &world)
	: world(world)
	{
		//Empty
	}

	void Frustum::update()
	{
		float clip[16];
		float t;
		const float *proj = glm::value_ptr(this->world.getPlayer().getProjMat());
		const float *modl = glm::value_ptr(this->world.getPlayer().getViewMat());
		clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
		clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
		clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
		clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
		clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
		clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
		clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
		clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
		clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
		clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
		clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
		clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
		clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
		clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
		clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
		clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
		// Right
		this->data[0][0] = clip[3] - clip[0];
		this->data[0][1] = clip[7] - clip[4];
		this->data[0][2] = clip[11] - clip[8];
		this->data[0][3] = clip[15] - clip[12];
		t = sqrt(this->data[0][0] * this->data[0][0] + this->data[0][1] * this->data[0][1] + this->data[0][2] * this->data[0][2]);
		this->data[0][0] /= t;
		this->data[0][1] /= t;
		this->data[0][2] /= t;
		this->data[0][3] /= t;
		//Left
		this->data[1][0] = clip[3] + clip[0];
		this->data[1][1] = clip[7] + clip[4];
		this->data[1][2] = clip[11] + clip[8];
		this->data[1][3] = clip[15] + clip[12];
		t = sqrt(this->data[1][0] * this->data[1][0] + this->data[1][1] * this->data[1][1] + this->data[1][2] * this->data[1][2]);
		this->data[1][0] /= t;
		this->data[1][1] /= t;
		this->data[1][2] /= t;
		this->data[1][3] /= t;
		//Bottom
		this->data[2][0] = clip[3] + clip[1];
		this->data[2][1] = clip[7] + clip[5];
		this->data[2][2] = clip[11] + clip[9];
		this->data[2][3] = clip[15] + clip[13];
		t = sqrt(this->data[2][0] * this->data[2][0] + this->data[2][1] * this->data[2][1] + this->data[2][2] * this->data[2][2]);
		this->data[2][0] /= t;
		this->data[2][1] /= t;
		this->data[2][2] /= t;
		this->data[2][3] /= t;
		//Top
		this->data[3][0] = clip[3] - clip[1];
		this->data[3][1] = clip[7] - clip[5];
		this->data[3][2] = clip[11] - clip[ 9];
		this->data[3][3] = clip[15] - clip[13];
		t = sqrt(this->data[3][0] * this->data[3][0] + this->data[3][1] * this->data[3][1] + this->data[3][2] * this->data[3][2]);
		this->data[3][0] /= t;
		this->data[3][1] /= t;
		this->data[3][2] /= t;
		this->data[3][3] /= t;
		//Back
		this->data[4][0] = clip[3] - clip[2];
		this->data[4][1] = clip[7] - clip[6];
		this->data[4][2] = clip[11] - clip[10];
		this->data[4][3] = clip[15] - clip[14];
		t = sqrt(this->data[4][0] * this->data[4][0] + this->data[4][1] * this->data[4][1] + this->data[4][2] * this->data[4][2]);
		this->data[4][0] /= t;
		this->data[4][1] /= t;
		this->data[4][2] /= t;
		this->data[4][3] /= t;
		//Front
		this->data[5][0] = clip[3] + clip[2];
		this->data[5][1] = clip[7] + clip[6];
		this->data[5][2] = clip[11] + clip[10];
		this->data[5][3] = clip[15] + clip[14];
		t = sqrt(this->data[5][0] * this->data[5][0] + this->data[5][1] * this->data[5][1] + this->data[5][2] * this->data[5][2]);
		this->data[5][0] /= t;
		this->data[5][1] /= t;
		this->data[5][2] /= t;
		this->data[5][3] /= t;
	}

	bool Frustum::check(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		for (uint8_t i = 0; i < 6; ++i)
		{
			if (this->data[i][0] * x1 + this->data[i][1] * y1 + this->data[i][2] * z1 + this->data[i][3] > 0)
				continue;
			if (this->data[i][0] * x2 + this->data[i][1] * y1 + this->data[i][2] * z1 + this->data[i][3] > 0)
				continue;
			if (this->data[i][0] * x1 + this->data[i][1] * y2 + this->data[i][2] * z1 + this->data[i][3] > 0)
				continue;
			if (this->data[i][0] * x2 + this->data[i][1] * y2 + this->data[i][2] * z1 + this->data[i][3] > 0)
				continue;
			if (this->data[i][0] * x1 + this->data[i][1] * y1 + this->data[i][2] * z2 + this->data[i][3] > 0)
				continue;
			if (this->data[i][0] * x2 + this->data[i][1] * y1 + this->data[i][2] * z2 + this->data[i][3] > 0)
				continue;
			if (this->data[i][0] * x1 + this->data[i][1] * y2 + this->data[i][2] * z2 + this->data[i][3] > 0)
				continue;
			if (this->data[i][0] * x2 + this->data[i][1] * y2 + this->data[i][2] * z2 + this->data[i][3] > 0)
				continue;
			return (false);
		}
		return (true);
	}

}