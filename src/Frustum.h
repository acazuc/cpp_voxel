#ifndef FRUSTUM_H
# define FRUSTUM_H

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
		bool check(float x1, float y1, float z1, float x2, float y2, float z2);

	};

}

#endif
