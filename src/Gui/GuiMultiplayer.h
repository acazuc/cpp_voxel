#ifndef GUI_MULTIPLAYER_H
# define GUI_MULTIPLAYER_H

# include "GuiMultiplayerEntry.h"
# include "GuiButton.h"
# include <librender/Shader/Sprite/ShaderSprite.h>
# include <librender/Window/Events.h>
# include <vector>

using librender::ShaderSprite;
using librender::MouseEvent;
using librender::KeyEvent;

namespace voxel
{

	class GuiMultiplayer
	{

	private:
		std::vector<GuiMultiplayerEntry*> entries;
		ShaderSprite topGradient;
		ShaderSprite botGradient;
		ShaderSprite background;
		ShaderSprite bgDarker;
		GuiButton refresh;
		GuiButton direct;
		GuiButton remove;
		GuiButton cancel;
		GuiButton join;
		GuiButton edit;
		GuiButton add;
		int32_t selected;
		static void refreshCallback(void *data);
		static void directCallback(void *data);
		static void removeCallback(void *data);
		static void cancelCallback(void *data);
		static void joinCallback(void *data);
		static void editCallback(void *data);
		static void addCallback(void *data);

	public:
		GuiMultiplayer();
		void draw();
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		bool mouseUp(MouseEvent &event);
		bool keyDown(KeyEvent &event);
		inline int32_t getSelected() {return this->selected;};
		inline void setSelected(int32_t selected) {this->selected = selected;};

	};

}

#endif
