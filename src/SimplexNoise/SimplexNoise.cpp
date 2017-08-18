#include "SimplexNoise.h"
#include <cmath>


namespace voxel
{

	SimplexNoise::SimplexNoise(uint32_t largestFeature, double persistence, int32_t seed)
	{
		this->largestFeature = largestFeature;
		this->persistence = persistence;
		this->seed = seed;
		this->octavesNumber = std::ceil(std::log10(this->largestFeature) / std::log10(2));
		this->octaves = new SimplexNoiseOctave[this->octavesNumber];
		this->frequencies = new double[this->octavesNumber];
		this->amplitudes = new double[this->octavesNumber];
		for (uint32_t i = 0; i < this->octavesNumber; ++i)
		{
			this->octaves[i].init(this->seed);
			this->frequencies[i] = pow(2, i);
			this->amplitudes[i] = pow(this->persistence, this->octavesNumber - i);
		}
	}

	SimplexNoise::~SimplexNoise()
	{
		delete[] (this->octaves);
		delete[] (this->frequencies);
		delete[] (this->amplitudes);
	}

	double SimplexNoise::get2(int32_t x, int32_t y)
	{
		double result = 0;
		for (uint32_t i = 0; i < this->octavesNumber; ++i)
			result += this->octaves[i].get2(x / this->frequencies[i], y / this->frequencies[i]) * this->amplitudes[i];
		return (result);
	}

	double SimplexNoise::get3(int32_t x, int32_t y, int32_t z)
	{
		double result = 0;
		for (uint32_t i = 0 ; i < this->octavesNumber; ++i)
		{
			double frequency = pow(2, i);
			double amplitude = pow(this->persistence, this->octavesNumber - i);
			result += this->octaves[i].get3(x / frequency, y / frequency, z / frequency) * amplitude;
		}
		return (result);
	}

}
