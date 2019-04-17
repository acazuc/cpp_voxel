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
				ChunkBlock *block = chunk.getBlock(x, top, z);
				if (block && block->getType() == 2)
				{
					if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 50)
						this->treeGenerator.generate(chunk, x, top + 1, z);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(x, top + 1, z, 37);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(x, top + 1, z, 38);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(x, top + 1, z, 39);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(x, top + 1, z, 40);
					else if (chunk.getWorld().getRandom()() < chunk.getWorld().getRandom().max() / 40)
						chunk.setBlockIfReplaceable(x, top + 1, z, 6);
				}
			}
		}
		for (size_t i = 0; i < 10; ++i)
		{
			int32_t x = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			int32_t z = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			if (x < CHUNK_WIDTH && z < CHUNK_WIDTH)
			{
				int32_t top = chunk.getTopBlock(x, z);
				this->ironOreGenerator.generate(chunk, x, std::max(chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * top, .2f) - 2, z);
			}
		}
		for (size_t i = 0; i < 10; ++i)
		{
			int32_t x = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			int32_t z = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			if (x < CHUNK_WIDTH && z < CHUNK_WIDTH)
			{
				int32_t top = chunk.getTopBlock(x, z);
				this->coalOreGenerator.generate(chunk, x, std::max(chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * top, 5.f) - 5, z);
			}
		}
		for (size_t i = 0; i < 5; ++i)
		{
			int32_t x = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			int32_t z = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			if (x < CHUNK_WIDTH && z < CHUNK_WIDTH)
			{
				int32_t top = chunk.getTopBlock(x, z);
				this->goldOreGenerator.generate(chunk, x, std::max(chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * top, 2.f) - 2, z);
			}
		}
		for (size_t i = 0; i < 4; ++i)
		{
			int32_t x = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			int32_t z = chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * CHUNK_WIDTH * 2;
			if (x < CHUNK_WIDTH && z < CHUNK_WIDTH)
			{
				int32_t top = chunk.getTopBlock(x, z);
				this->diamondOreGenerator.generate(chunk, x, std::max(chunk.getWorld().getRandom()() / (float)chunk.getWorld().getRandom().max() * top, 2.f) - 2, z);
			}
		}
	}

	Biome *Biome::setName(std::string name)
	{
		this->name = name;
		return this;
	}

}
