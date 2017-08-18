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
		double *frequencies;
		double *amplitudes;
		double persistence;
		uint32_t largestFeature;
		uint32_t octavesNumber;
		int32_t seed;

	public:
		SimplexNoise(uint32_t largestFeature, double persistance, int32_t seed);
		~SimplexNoise();
		double get2(int32_t x, int32_t y);
		double get3(int32_t x, int32_t y, int32_t z);

	};

}

#endif
