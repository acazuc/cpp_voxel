#ifndef TITLE_SCREEN_H
# define TITLE_SCREEN_H

# include "Gui/GuiButton.h"
# include "Screen.h"
# include <librender/Shader/Sprite/ShaderSprite.h>

using librender::ShaderSprite;

namespace voxel
{

	class TitleScreen : public Screen
	{

	private:
		ShaderSprite background;
		ShaderSprite logo;
		GuiButton singleplayer;
		GuiButton multiplayer;
		GuiButton texturePack;
		GuiButton options;
		GuiButton quit;

	public:
		TitleScreen();
		void draw();

	};

}

#endif