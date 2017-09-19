#ifndef GUI_H
# define GUI_H

# include "./GuiCross.h"
# include "./GuiBar.h"
# include <librender/Texture.h>

using librender::Texture;

namespace voxel
{

	class Gui
	{

	private:
		static Texture *gui;
		glm::mat4 mat;
		GuiCross cross;
		GuiBar bar;

	public:
		Gui();
		~Gui();
		void draw();
		inline glm::mat4 &getMat() {return (this->mat);};
		static void init();
		static inline Texture *getGui() {return (gui);};
	};

}

#endif
