#ifndef WORLD_SCREEN_GUI_H
# define WORLD_SCREEN_GUI_H

# include "Gui/GuiLagometer.h"
# include "Gui/GuiInventory.h"
# include "Gui/GuiHearts.h"
# include "Gui/GuiCross.h"
# include "Gui/GuiLabel.h"
# include "Gui/GuiBar.h"
# include <librender/Window/Events.h>

using librender::ScrollEvent;
using librender::KeyEvent;

namespace voxel
{

	class WorldScreen;

	class WorldScreenGui
	{

	private:
		ShaderSprite water;
		GuiLagometer lagometer;
		GuiInventory inventory;
		WorldScreen &worldScreen;
		GuiHearts hearts;
		GuiLabel focusedLabel;
		GuiLabel fpsLabel;
		GuiLabel posLabel;
		GuiCross cross;
		GuiBar bar;
		int64_t lastChunkUpdates;
		int64_t lastFps;
		bool inventoryEnabled;

	public:
		WorldScreenGui(WorldScreen &worldScreen);
		void draw();
		void mouseScroll(ScrollEvent &event);
		void keyDown(KeyEvent &event);

	};

}

#endif
