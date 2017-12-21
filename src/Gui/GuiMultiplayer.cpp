#include "GuiMultiplayer.h"
#include "TitleScreen/TitleScreen.h"

namespace voxel
{

	GuiMultiplayer::GuiMultiplayer()
	{
		//
	}

	void GuiMultiplayer::draw()
	{
		//
	}

	void GuiMultiplayer::mouseMove(bool &alreadyHovered)
	{
		(void)alreadyHovered;
	}

	bool GuiMultiplayer::mouseDown(MouseEvent &event)
	{
		(void)event;
		return (false);
	}

	bool GuiMultiplayer::mouseUp(MouseEvent &event)
	{
		(void)event;
		return (false);
	}

	bool GuiMultiplayer::keyDown(KeyEvent &event)
	{
		if (event.key == GLFW_KEY_ESCAPE)
		{
			TitleScreen::setMode(TITLE_SCREEN_TITLE);
			return (true);
		}
		return (false);
	}

}
