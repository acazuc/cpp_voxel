#ifndef SCREEN_H
# define SCREEN_H

# include <librender/Window/Events.h>

using librender::ScrollEvent;
using librender::MouseEvent;
using librender::CharEvent;
using librender::KeyEvent;

namespace voxel
{

	class Screen
	{

	public:
		virtual ~Screen() {};
		virtual void tick() {};
		virtual void draw() {};
		virtual void mouseScroll(ScrollEvent &event) {(void)event;};
		virtual void mouseMove() {};
		virtual void mouseDown(MouseEvent &event) {(void)event;};
		virtual void mouseUp(MouseEvent &event) {(void)event;};
		virtual void keyDown(KeyEvent &event) {(void)event;};
		virtual void keyPress(KeyEvent &event) {(void)event;};
		virtual void keyUp(KeyEvent &event) {(void)event;};
		virtual void charEntered(CharEvent &event) {(void)event;};
		virtual void langChanged() {};
		virtual void windowResized() {};

	};

}

#endif
