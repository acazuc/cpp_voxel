#ifndef GUI_OPTIONS_H
# define GUI_OPTIONS_H

# include "GuiOptionsChat.h"
# include "Gui/GuiButton.h"
# include "Gui/GuiSlider.h"
# include <librender/Window/Events.h>

using librender::MouseEvent;
using librender::KeyEvent;

namespace voxel
{

	class GuiOptions
	{

	private:
		GuiOptionsChat chatOptions;
		GuiButton resources;
		GuiButton controls;
		GuiButton language;
		GuiButton sound;
		GuiButton video;
		GuiButton chat;
		GuiButton done;
		GuiButton skin;
		GuiSlider fov;
		GuiLabel title;
		static void resourcesCallback(void *data);
		static void controlsCallback(void *data);
		static void languageCallback(void *data);
		static void soundCallback(void *data);
		static void videoCallback(void *data);
		static void chatCallback(void *data);
		static void doneCallback(void *data);
		static void skinCallback(void *data);
		static void fovCallback(float per, void *data);

	public:
		GuiOptions();
		void draw();
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		bool mouseUp(MouseEvent &event);
		bool keyDown(KeyEvent &event);

	};

}

#endif
