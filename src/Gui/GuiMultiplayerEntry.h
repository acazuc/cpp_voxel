#ifndef GUI_NULTIPLAYER_ENTRY_H
# define GUI_MULTIPLAYER_ENTRY_H

# include "GuiLabel.h"
# include <librender/Shader/Sprite/ShaderSprite.h>
# include <librender/Window/Events.h>

using librender::ShaderSprite;
using librender::MouseEvent;

namespace voxel
{

	class GuiMultiplayer;

	class GuiMultiplayerEntry
	{

	private:
		GuiMultiplayer &parent;

	public:
		GuiMultiplayerEntry(GuiMultiplayer &parent, int32_t pos);
		void draw(Mat4 &mat);
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);

	};

}

#endif
