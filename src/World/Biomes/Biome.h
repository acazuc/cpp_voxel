#ifndef BIOME_H
# define BIOME_H

# include "World/Generators/GeneratorMinable.h"
# include "World/Generators/GeneratorTree.h"
# include <string>

namespace voxel
{

	class Biome
	{

	private:
		GeneratorMinable diamondOreGenerator;
		GeneratorMinable coalOreGenerator;
		GeneratorMinable ironOreGenerator;
		GeneratorMinable goldOreGenerator;
		GeneratorTree treeGenerator;
		std::string name;
		uint8_t id;

	public:
		Biome(uint8_t id);
		Biome *setName(std::string name);
		void generate(Chunk &chunk);
		inline std::string &getName() {return (this->name);};

	};

}

#endif
