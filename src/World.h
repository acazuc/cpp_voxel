#ifndef WORLD_H
# define WORLD_H

# include "Player.h"
# include "Chunk.h"
# include <vector>

namespace voxel
{

	class World
	{

	private:
		std::vector<Chunk*> chunks;
		Player player;

	public:
		World();
		void tick();
		void draw();
		Chunk *getChunk(int32_t x, int32_t z);
		inline Player &getPlayer() {return (this->player);};

	};

}

#endif
