#include "Biome.h"
#include "World/World.h"

namespace voxel
{

	Biome::Biome(uint8_t id)
	: id(id)
	{
		this->diamondOreGenerator.setBlock(56);
		this->diamondOreGenerator.setSize(2);
		this->coalOreGenerator.setBlock(16);
		this->coalOreGenerator.setSize(5);
		this->ironOreGenerator.setBlock(15);
		this->ironOreGenerator.setSize(2);
		this->goldOreGenerator.setBlock(14);
		this->goldOreGenerator.setSize(2);
	}

	void Biome::generate(Chunk &chunk)
	{
		for (int32_t x = 0; x < CHUNK_WIDTH; ++x)
		{
			for (int32_t z = 0; z < CHUNK_WIDTH; ++z)
			{
				int32_t top = chunk.getTopBlock(x, z);
				ChunkBlock *block = chunk.getBlock(glm::vec3(x, top, z));
				if (block && block->getType() == 2)
				{
					if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 100)
						this->treeGenerator.generate(chunk, x, top + 1, z);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(glm::vec3(x, top + 1, z), 37);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(glm::vec3(x, top + 1, z), 38);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(glm::vec3(x, top + 1, z), 39);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(glm::vec3(x, top + 1, z), 40);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(glm::vec3(x, top + 1, z), 6);
				}
				if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 500)
					this->ironOreGenerator.generate(chunk, x, chunk.getWorld().getRandom()() / (chunk.getWorld().getRandom().max() / 10.) * CHUNK_HEIGHT, z);
				if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 500)
					this->coalOreGenerator.generate(chunk, x, chunk.getWorld().getRandom()() / (chunk.getWorld().getRandom().max() / 10.) * CHUNK_HEIGHT, z);
				if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 1000)
					this->goldOreGenerator.generate(chunk, x, chunk.getWorld().getRandom()() / (chunk.getWorld().getRandom().max() / 10.) * CHUNK_HEIGHT, z);
				if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 1000)
					this->diamondOreGenerator.generate(chunk, x, chunk.getWorld().getRandom()() / (chunk.getWorld().getRandom().max() / 10.) * CHUNK_HEIGHT, z);
			}
		}
	}

	Biome *Biome::setName(std::string name)
	{
		this->name = name;
		return (this);
	}

}
