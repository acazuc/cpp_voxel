#ifndef FRUSTUM_H
# define FRUSTUM_H

# include "AABB.h"
# include <librender/Mat/Mat4.h>
# include <librender/Vec/Vec4.h>
# include <vector>
# include <cmath>

using librender::Mat4;
using librender::Vec4;

namespace voxel
{

	class World;

	class Frustum
	{

	private:
		std::vector<Vec4> planes;

	public:
		void update(Mat4 &vp);
		bool check(AABB &aabb);

	};

}

#endif
