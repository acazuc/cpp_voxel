#include "Biomes.h"
#include <cstring>

namespace voxel
{

	Biome **Biomes::biomes;

	void Biomes::init()
	{
		biomes = new Biome*[255];
		std::memset(biomes, 0, sizeof(*biomes) * 255);
		biomes[0] = (new Biome(0))->setName("ocean");
		biomes[1] = (new Biome(1))->setName("plains");
		biomes[2] = (new Biome(2))->setName("desert");
		biomes[3] = (new Biome(3))->setName("extreme_hills");
		biomes[4] = (new Biome(4))->setName("forest");
		biomes[5] = (new Biome(5))->setName("taiga");
		biomes[6] = (new Biome(6))->setName("swampland");
		biomes[7] = (new Biome(7))->setName("river");
		biomes[8] = (new Biome(8))->setName("hell");
		biomes[9] = (new Biome(9))->setName("sky");
		biomes[10] = (new Biome(10))->setName("frozen_ocean");
		biomes[11] = (new Biome(11))->setName("frozen_river");
		biomes[12] = (new Biome(12))->setName("ice_flats");
		biomes[13] = (new Biome(13))->setName("ice_mountains");
		biomes[14] = (new Biome(14))->setName("mushroom_island");
	}

	void Biomes::clear()
	{
		for (uint8_t i = 0; i < 255; ++i)
			delete (biomes[i]);
		delete[] (biomes);
	}

}
