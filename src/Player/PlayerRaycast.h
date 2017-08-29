#ifndef PLAYER_RAYCAST_H
# define PLAYER_RAYCAST_H

# include <librender/Shader/DataBuffer.h>

using librender::DataBuffer;

namespace voxel
{

	class Player;

	class PlayerRaycast
	{

	private:
		Player &player;
		DataBuffer vertexesBuffer;
		DataBuffer colorsBuffer;
		uint8_t face;
		int32_t x;
		int32_t y;
		int32_t z;
		bool found;
		void raycast();

	public:
		PlayerRaycast(Player &player);
		~PlayerRaycast();
		void tick();
		void draw();

	};

}

#endif
