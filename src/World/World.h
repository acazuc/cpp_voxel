#ifndef WORLD_H
# define WORLD_H

# include "Particles/ParticlesManager.h"
# include "Entities/EntitiesManager.h"
# include "Noise/SimplexNoise.h"
# include "Player/Player.h"
# include "ChunkUpdater.h"
# include "ChunkLoader.h"
# include "Platform.h"
# include "Frustum.h"
# include "Clouds.h"
# include "Skybox.h"
# include "Region.h"
# include "Chunk.h"
# include <random>
# include <vector>
# include <list>
# ifdef PLATFORM_WINDOWS
#  include <mutex.h>
# else
#  include <mutex>
# endif

namespace voxel
{

	class World
	{

	private:
		std::linear_congruential_engine<uint32_t, 48271, 0, UINT_MAX> random;
		std::vector<VertexBuffer*> buffersToDelete;
		std::recursive_mutex chunksMutex;
		std::vector<Region*> regions;
		std::list<Chunk*> chunksToUpdate;
		ParticlesManager particlesManager;
		EntitiesManager entitiesManager;
		ChunkUpdater chunkUpdater;
		ChunkLoader chunkLoader;
		SimplexNoise noise;
		Frustum frustum;
		Player player;
		Clouds clouds;
		Skybox skybox;
		int64_t lastRegionCheck;
		void checkEmptyRegions();
		void clearBuffersToDelete();

	public:
		World();
		~World();
		void tick();
		void draw();
		void getAABBs(AABB &aabb, std::vector<AABB> &aabbs);
		void addChunk(Chunk *chunk);
		Chunk *getChunk(int32_t x, int32_t z);
		ChunkBlock *getBlock(glm::vec3 pos);
		uint8_t getLight(glm::vec3 pos);
		inline std::linear_congruential_engine<uint32_t, 48271, 0, UINT_MAX> &getRandom() {return (this->random);};
		inline std::vector<VertexBuffer*> &getBuffersToDelete() {return (this->buffersToDelete);};
		inline std::recursive_mutex &getChunksMutex() {return (this->chunksMutex);};
		inline std::vector<Region*> &getRegions() {return (this->regions);};
		inline std::list<Chunk*> &getChunksToUpdate() {return (this->chunksToUpdate);};
		inline ParticlesManager &getParticlesManager() {return (this->particlesManager);};
		inline EntitiesManager &getEntitiesManager() {return (this->entitiesManager);};
		inline SimplexNoise &getNoise() {return (this->noise);};
		inline Frustum &getFrustum() {return (this->frustum);};
		inline Player &getPlayer() {return (this->player);};

	};

}

#endif
