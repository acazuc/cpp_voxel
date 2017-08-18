#ifndef WORLD_H
# define WORLD_H

# include "SimplexNoise/SimplexNoise.h"
# include "Frustum.h"
# include "Player.h"
# include "Chunk.h"
# include <vector>

namespace voxel
{

	class World
	{

	private:
		std::vector<Chunk*> chunks;
		SimplexNoise noise;
		Frustum frustum;
		Player player;

	public:
		World();
		~World();
		void tick();
		void draw();
		Chunk *getChunk(int32_t x, int32_t z);
		inline SimplexNoise &getNoise() {return (this->noise);};
		inline Frustum &getFrustum() {return (this->frustum);};
		inline Player &getPlayer() {return (this->player);};

	};

}

#endif
