#ifndef GUI_SINGLEPLAYER_ENTRY_H
# define GUI_SINGLEPLAYER_ENTRY_H

# include "GuiLabel.h"
# include <librender/Shader/Sprite/ShaderSprite.h>
# include <librender/Window/Events.h>

using librender::ShaderSprite;
using librender::MouseEvent;

namespace voxel
{

	class GuiSingleplayer;

	class GuiSingleplayerEntry
	{

	private:
		GuiSingleplayer &parent;
		ShaderSprite background;
		ShaderSprite border;
		ShaderSprite image;
		GuiLabel dirDate;
		GuiLabel other;
		GuiLabel name;
		int32_t pos;
		bool hover;

	public:
		GuiSingleplayerEntry(GuiSingleplayer &parent, int32_t pos);
		void draw(Mat4 &mat);
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);

	};

}

#endif
