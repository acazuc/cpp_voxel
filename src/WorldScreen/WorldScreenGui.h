#ifndef WORLD_SCREEN_GUI_H
# define WORLD_SCREEN_GUI_H

# include "Gui/GuiHearts.h"
# include "Gui/GuiCross.h"
# include "Gui/GuiBar.h"

namespace voxel
{

	class WorldScreen;

	class WorldScreenGui
	{

	private:
		ShaderSprite water;
		WorldScreen &worldScreen;
		GuiHearts hearts;
		GuiCross cross;
		GuiBar bar;

	public:
		WorldScreenGui(WorldScreen &worldScreen);
		void draw();

	};

}

#endif
