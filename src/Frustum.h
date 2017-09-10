#ifndef FRUSTUM_H
# define FRUSTUM_H

# include "AABB.h"
# include <glm/vec4.hpp>

namespace voxel
{

	class World;

	class Frustum
	{

	private:
		World &world;
		glm::vec4 data[6];

	public:
		Frustum(World &world);
		void update();
		bool check(AABB &aabb);

	};

}

#endif
