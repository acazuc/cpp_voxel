#ifndef GUI_MULTIPLAYER_H
# define GUI_MULTIPLAYER_H

# include <librender/Window/Events.h>

using librender::MouseEvent;
using librender::KeyEvent;

namespace voxel
{

	class GuiMultiplayer
	{

	private:
		//

	public:
		GuiMultiplayer();
		void draw();
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		bool mouseUp(MouseEvent &event);
		bool keyDown(KeyEvent &event);

	};

}

#endif
