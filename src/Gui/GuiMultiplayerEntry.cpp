#include "GuiMultiplayerEntry.h"

namespace voxel
{

	GuiMultiplayerEntry::GuiMultiplayerEntry(GuiMultiplayer &parent, int32_t pos)
	: parent(parent)
	{
		(void)pos;
	}

	void GuiMultiplayerEntry::draw(Mat4 &mat)
	{
		(void)mat;
	}

	void GuiMultiplayerEntry::mouseMove(bool &alreadyHovered)
	{
		(void)alreadyHovered;
	}

	bool GuiMultiplayerEntry::mouseDown(MouseEvent &event)
	{
		(void)event;
		return false;
	}

}
