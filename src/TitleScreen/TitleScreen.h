#ifndef TITLE_SCREEN_H
# define TITLE_SCREEN_H

# include "Gui/Options/GuiOptions.h"
# include "Gui/GuiSingleplayer.h"
# include "Gui/GuiMultiplayer.h"
# include "Gui/GuiLagometer.h"
# include "TitleScreenMode.h"
# include "Gui/GuiTitle.h"
# include "Screen.h"
# include <librender/Shader/Sprite/ShaderSprite.h>

using librender::ShaderSprite;

namespace voxel
{

	class TitleScreen : public Screen
	{

	private:
		static enum TitleScreenMode mode;
		GuiSingleplayer singleplayer;
		GuiMultiplayer multiplayer;
		ShaderSprite background;
		GuiLagometer lagometer;
		GuiOptions options;
		GuiTitle title;

	public:
		TitleScreen();
		void draw();
		void mouseMove();
		void mouseDown(MouseEvent &event);
		void mouseUp(MouseEvent &event);
		void keyDown(KeyEvent &event);
		static inline void setMode(enum TitleScreenMode mode) {TitleScreen::mode = mode;};

	};

}

#endif
