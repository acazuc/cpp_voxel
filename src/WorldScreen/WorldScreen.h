#ifndef WORLD_SCREEN_H
# define WORLD_SCREEN_H

# include "WorldScreenGui.h"
# include "World/World.h"
# include "Screen.h"

namespace voxel
{

	class WorldScreen : public Screen
	{

	private:
		WorldScreenGui gui;
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
