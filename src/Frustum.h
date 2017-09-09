#ifndef FRUSTUM_H
# define FRUSTUM_H

# include "AABB.h"

namespace voxel
{

	class World;

	class Frustum
	{

	private:
		World &world;
		float data[6][4];

	public:
		Frustum(World &world);
		void update();
		bool check(AABB &aabb);

	};

}

#endif
