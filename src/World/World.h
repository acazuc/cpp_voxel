#ifndef WORLD_H
# define WORLD_H

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
		std::vector<VertexBuffer*> VBOToDelete;
		std::vector<VertexArray*> VAOToDelete;
		std::recursive_mutex chunksMutex;
		std::vector<Region*> regions;
		std::list<Chunk*> chunksToUpdate;
		std::minstd_rand random;
		ChunkUpdater chunkUpdater;
		ChunkLoader chunkLoader;
		SimplexNoise biomeTempNoise;
		SimplexNoise biomeRainNoise;
		SimplexNoise noise;
		Frustum frustum;
		Player player;
		Clouds clouds;
		Skybox skybox;
		int64_t lastRegionCheck;
		void checkEmptyRegions();
		void clearVBOToDelete();
		void clearVAOToDelete();

	public:
		World();
		~World();
		void save();
		void tick();
		void draw();
		void getAABBs(AABB &aabb, std::vector<AABB> &aabbs);
		void setBlock(int32_t x, int32_t y, int32_t z, uint8_t type);
		void setBlockIfReplaceable(int32_t x, int32_t y, int32_t z, uint8_t type);
		void generateChunk(int32_t x, int32_t z);
		void addChunk(Chunk *chunk);
		Chunk *getChunk(int32_t x, int32_t z);
		ChunkBlock *getBlock(int32_t x, int32_t y, int32_t z);
		uint8_t getLight(int32_t x, int32_t y, int32_t z);
		static int32_t getChunkCoord(int32_t coord);
		static int32_t getRegionCoord(int32_t coord);
		inline std::vector<VertexBuffer*> &getVBOToDelete() {return (this->VBOToDelete);};
		inline std::vector<VertexArray*> &getVAOToDelete() {return (this->VAOToDelete);};
		inline std::recursive_mutex &getChunksMutex() {return (this->chunksMutex);};
		inline std::vector<Region*> &getRegions() {return (this->regions);};
		inline std::list<Chunk*> &getChunksToUpdate() {return (this->chunksToUpdate);};
		inline std::minstd_rand &getRandom() {return (this->random);};
		inline SimplexNoise &getBiomeTempNoise() {return (this->biomeTempNoise);};
		inline SimplexNoise &getBiomeRainNoise() {return (this->biomeRainNoise);};
		inline SimplexNoise &getNoise() {return (this->noise);};
		inline Frustum &getFrustum() {return (this->frustum);};
		inline Player &getPlayer() {return (this->player);};

	};

}

#endif
