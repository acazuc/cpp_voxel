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
		void mouseScroll(ScrollEvent &event);
		void keyDown(KeyEvent &event);
		inline World *getWorld() {return (this->world);};

	};

}

#endif
