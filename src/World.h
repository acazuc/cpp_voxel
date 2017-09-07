#ifndef WORLD_H
# define WORLD_H

# include "Noise/SimplexNoise.h"
# include "ChunkLoader.h"
# include "Frustum.h"
# include "Player.h"
# include "Clouds.h"
# include "Skybox.h"
# include "Region.h"
# include "Chunk.h"
# include <vector>
# include <mutex>

namespace voxel
{

	class World
	{

	private:
		std::vector<VertexBuffer*> buffersToDelete;
		std::recursive_mutex chunksMutex;
		std::vector<Region*> regions;
		std::vector<Chunk*> chunks;
		ChunkLoader chunkLoader;
		SimplexNoise noise;
		Frustum frustum;
		Player player;
		Clouds clouds;
		Skybox skybox;

	public:
		World();
		~World();
		void update();
		void tick();
		void draw();
		Chunk *getChunk(int32_t x, int32_t z);
		void getAABBs(AABB &aabb, std::vector<AABB> &aabbs);
		void addChunk(Chunk *chunk);
		inline std::vector<VertexBuffer*> &getBuffersToDelete() {return (this->buffersToDelete);};
		inline std::recursive_mutex &getChunksMutex() {return (this->chunksMutex);};
		inline std::vector<Region*> &getRegions() {return (this->regions);};
		inline std::vector<Chunk*> &getChunks() {return (this->chunks);};
		inline SimplexNoise &getNoise() {return (this->noise);};
		inline Frustum &getFrustum() {return (this->frustum);};
		inline Player &getPlayer() {return (this->player);};

	};

}

#endif
