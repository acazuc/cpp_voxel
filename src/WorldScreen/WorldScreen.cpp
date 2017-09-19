#include "WorldScreen.h"
#include "Gui/Gui.h"

namespace voxel
{

	WorldScreen::WorldScreen(World *world)
	: world(world)
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
		Gui::updateMat();
		this->world->draw();
	}

}
