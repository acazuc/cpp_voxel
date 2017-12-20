#ifndef GUI_TITLE_H
# define GUI_TITLE_H

# include "Gui/GuiButton.h"
# include <librender/Shader/Sprite/ShaderSprite.h>

using librender::ShaderSprite;

namespace voxel
{

	class GuiTitle
	{

	private:
		ShaderSprite logo;
		GuiButton singleplayer;
		GuiButton multiplayer;
		GuiButton texturePack;
		GuiButton options;
		GuiButton quit;
		static void singleplayerCallback();
		static void multiplayerCallback();
		static void texturePackCallback();

	public:
		GuiTitle();
		void draw();
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);

	};

}

#endif
