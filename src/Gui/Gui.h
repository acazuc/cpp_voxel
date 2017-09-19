#ifndef GUI_H
# define GUI_H

# include "./GuiInventory.h"
# include "./GuiHearts.h"
# include "./GuiCross.h"
# include "./GuiBar.h"
# include <librender/Texture.h>

using librender::Texture;

namespace voxel
{

	class Gui
	{

	private:
		static Texture *inventoryTex;
		static Texture *iconsTex;
		static Texture *guiTex;
		glm::mat4 mat;
		GuiInventory inventory;
		GuiHearts hearts;
		GuiCross cross;
		GuiBar bar;

	public:
		Gui();
		~Gui();
		void draw();
		inline glm::mat4 &getMat() {return (this->mat);};
		static void init();
		static inline Texture *getInventoryTex() {return (inventoryTex);};
		static inline Texture *getIconsTex() {return (iconsTex);};
		static inline Texture *getGuiTex() {return (guiTex);};
	};

}

#endif
