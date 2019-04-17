#include "SimplexNoise.h"
#include <cmath>

namespace voxel
{

	SimplexNoise::SimplexNoise(uint32_t octavesNumber, double persistence, int32_t seed)
	: octavesNumber(octavesNumber)
	, persistence(persistence)
	, seed(seed)
	{
		this->octavesNumber = octavesNumber;
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

	double SimplexNoise::get2(double x, double y)
	{
		double result = 0;
		for (uint32_t i = 0; i < this->octavesNumber; ++i)
			result += this->octaves[i].get2(x / this->frequencies[i], y / this->frequencies[i]) * this->amplitudes[i];
		return result;
	}

	double SimplexNoise::get3(double x, double y, double z)
	{
		double result = 0;
		for (uint32_t i = 0 ; i < this->octavesNumber; ++i)
			result += this->octaves[i].get3(x / this->frequencies[i], y / this->frequencies[i], z / this->frequencies[i]) * this->amplitudes[i];
		return result;
	}

	double SimplexNoise::get4(double x, double y, double z, double w)
	{
		double result = 0;
		for (uint32_t i = 0; i < this->octavesNumber; ++i)
			result += this->octaves[i].get4(x / this->frequencies[i], y / this->frequencies[i], z / this->frequencies[i], w / this->frequencies[i]) * this->amplitudes[i];
		return result;
	}

}
