#include "WorldScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	WorldScreen::WorldScreen(World *world)
	: gui(*this)
	, world(world)
	{
		glfwSetInputMode(Main::getWindow()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	void WorldScreen::mouseScroll(ScrollEvent &event)
	{
		this->gui.mouseScroll(event);
	}

	void WorldScreen::keyDown(KeyEvent &event)
	{
		this->gui.keyDown(event);
	}

}
