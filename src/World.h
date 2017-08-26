#ifndef WORLD_H
# define WORLD_H

# include "SimplexNoise/SimplexNoise.h"
# include "ChunkLoader.h"
# include "Frustum.h"
# include "Player.h"
# include "Clouds.h"
# include "Chunk.h"
# include <vector>
# include <mutex>

namespace voxel
{

	class World
	{

	private:
		std::vector<Chunk*> chunks;
		ChunkLoader chunkLoader;
		std::mutex chunksMutex;
		SimplexNoise noise;
		Frustum frustum;
		Player player;
		Clouds clouds;

	public:
		World();
		~World();
		void tick();
		void draw();
		Chunk *getChunk(int32_t x, int32_t z);
		void addChunk(Chunk *chunk);
		inline std::vector<Chunk*> &getChunks() {return (this->chunks);};
		inline std::mutex &getChunksMutex() {return (this->chunksMutex);};
		inline SimplexNoise &getNoise() {return (this->noise);};
		inline Frustum &getFrustum() {return (this->frustum);};
		inline Player &getPlayer() {return (this->player);};

	};

}

#endif
