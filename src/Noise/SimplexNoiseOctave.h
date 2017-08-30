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
		double w;
	};

	class SimplexNoiseOctave
	{

	private:
		uint8_t perm[512];
		uint8_t permMod12[512];
		double dot2(SimplexNoiseGrad *grad, double x, double y);
		double dot3(SimplexNoiseGrad *grad, double x, double y, double z);
		double dot4(SimplexNoiseGrad *grad, double x, double y, double z, double w);
		int32_t fastFloor(double val);

	public:
		void init(uint32_t seed);
		double get2(double x, double y);
		double get3(double x, double y, double z);
		double get4(double x, double y, double z, double w);

	};

}

#endif
