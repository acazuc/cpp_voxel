#ifndef FRUSTUM_H
# define FRUSTUM_H

# include "AABB.h"
# include <glm/mat4x4.hpp>
# include <glm/vec4.hpp>

namespace voxel
{

	class World;

	class Frustum
	{

	private:
		glm::vec4 data[6];

	public:
		void update(glm::mat4 &vp);
		bool check(AABB &aabb);

	};

}

#endif
