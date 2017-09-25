#include "Biome.h"

namespace voxel
{

	Biome::Biome(uint8_t id)
	: id(id)
	{
		 //Empty
	}

	Biome *Biome::setName(std::string name)
	{
		this->name = name;
		return (this);
	}

}
