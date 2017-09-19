#ifndef WORLD_SCREEN_H
# define WORLD_SCREEN_H

# include "World/World.h"
# include "Screen.h"

namespace voxel
{

	class WorldScreen : public Screen
	{

	private:
		World *world;

	public:
		WorldScreen(World *world);
		~WorldScreen();
		void tick();
		void draw();
		inline World *getWorld() {return (this->world);};

	};

}

#endif
