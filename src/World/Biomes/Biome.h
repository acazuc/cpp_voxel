#ifndef BIOME_H
# define BIOME_H

# include <string>

namespace voxel
{

	class Biome
	{

	private:
		std::string name;
		uint8_t id;

	public:
		Biome(uint8_t id);
		Biome *setName(std::string name);
		inline std::string &getName() {return (this->name);};

	};

}

#endif
