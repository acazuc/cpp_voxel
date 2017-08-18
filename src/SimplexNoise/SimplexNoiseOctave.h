#ifndef SIMPLEX_NOISE_OCTAVE_H
# define SIMPLEX_NOISE_OCTAVE_H

# include <cstdint>

namespace voxel
{

	struct SimplexNoiseGrad
	{
		double x;
		double y;
		double z;
	};

	class SimplexNoiseOctave
	{

	private:
		uint8_t perm[512];
		uint8_t permMod12[512];
		double dot2(SimplexNoiseGrad *grad, double x, double y);
		double dot3(SimplexNoiseGrad *grad, double x, double y, double z);
		int32_t fastFloor(double val);

	public:
		void init(uint32_t seed);
		double get2(double xin, double yin);
		double get3(double xin, double yin, double zin);

	};

}

#endif
