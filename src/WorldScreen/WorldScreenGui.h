#ifndef WORLD_SCREEN_GUI_H
# define WORLD_SCREEN_GUI_H

# include "Gui/GuiLagometer.h"
# include "Gui/GuiHearts.h"
# include "Gui/GuiCross.h"
# include "Gui/GuiLabel.h"
# include "Gui/GuiBar.h"

namespace voxel
{

	class WorldScreen;

	class WorldScreenGui
	{

	private:
		ShaderSprite water;
		GuiLagometer lagometer;
		WorldScreen &worldScreen;
		GuiHearts hearts;
		GuiLabel focusedLabel;
		GuiLabel fpsLabel;
		GuiLabel posLabel;
		GuiCross cross;
		GuiBar bar;
		int64_t lastChunkUpdates;
		int64_t lastFps;

	public:
		WorldScreenGui(WorldScreen &worldScreen);
		void draw();

	};

}

#endif
