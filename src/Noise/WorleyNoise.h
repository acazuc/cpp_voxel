#ifndef WORLEY_NOISE_H
# define WORLEY_NOISE_H

# include <librender/Vec/Vec3.h>
# include <cstdint>

using librender::Vec3;

namespace voxel
{

	class WorleyNoise
	{

	private:
		static Vec3 permute(Vec3 x);
		static Vec3 dist(Vec3 x, Vec3 y);

	public:
		static double get2(double x, double y);

	};

}

#endif
