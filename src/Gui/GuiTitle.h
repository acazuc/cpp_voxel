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
		static void singleplayerCallback(void *data);
		static void multiplayerCallback(void *data);
		static void texturePackCallback(void *data);
		static void optionsCallback(void *data);
		static void quitCallback(void *data);

	public:
		GuiTitle();
		void draw();
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		bool mouseUp(MouseEvent &event);

	};

}

#endif
