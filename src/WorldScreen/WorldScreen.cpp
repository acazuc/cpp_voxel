#include "WorldScreen.h"
#include "Gui/Gui.h"

namespace voxel
{

	WorldScreen::WorldScreen(World *world)
	: gui(*this)
	, world(world)
	{
		//
	}

	WorldScreen::~WorldScreen()
	{
		delete (this->world);
	}

	void WorldScreen::tick()
	{
		this->world->tick();
	}

	void WorldScreen::draw()
	{
		this->world->draw();
		this->gui.draw();
	}

}
