#ifndef FRUSTUM_H
# define FRUSTUM_H

# include "AABB.h"
# include <librender/Mat/Mat4.h>
# include <librender/Vec/Vec4.h>

using librender::Mat4;
using librender::Vec4;

namespace voxel
{

	class World;

	class Frustum
	{

	private:
		Vec4 data[6];

	public:
		void update(Mat4 &vp);
		bool check(AABB &aabb);

	};

}

#endif
