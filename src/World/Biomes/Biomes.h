#ifndef BIOMES_H
# define BIOMES_H

# include "Biome.h"

namespace voxel
{

	class Biomes
	{

	private:
		static Biome **biomes;

	public:
		static void init();
		static void clear();
		static uint8_t getBiomeFor(float temp, float rain);
		static inline Biome *getBiome(uint8_t id) {return biomes[id];};

	};

}

#endif
