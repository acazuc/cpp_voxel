#ifndef SIMPLEX_NOISE_H
# define SIMPLEX_NOISE_H

# include "SimplexNoiseOctave.h"
# include <cstdint>

namespace voxel
{

	class SimplexNoise
	{

	private:
		SimplexNoiseOctave *octaves;
		uint32_t largestFeature;
		uint32_t octavesNumber;
		double *frequencies;
		double *amplitudes;
		double persistence;
		int32_t seed;

	public:
		SimplexNoise(uint32_t largestFeature, double persistance, int32_t seed);
		~SimplexNoise();
		double get2(double x, double y);
		double get3(double x, double y, double z);
		double get4(double x, double y, double z, double w);

	};

}

#endif
