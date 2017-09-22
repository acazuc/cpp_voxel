#ifndef GUI_H
# define GUI_H

# include "./GuiInventory.h"
# include "./GuiHearts.h"
# include "./GuiCross.h"
# include "./GuiBar.h"
# include <librender/Texture.h>

using librender::Texture;
using librender::Color;

namespace voxel
{

	class Gui
	{

	private:
		static glm::mat4 mat;
		static Texture *inventoryTex;
		static Texture *iconsTex;
		static Texture *waterTex;
		static Texture *logoTex;
		static Texture *guiTex;
		static Texture *bgTex;
		GuiInventory inventory;
		GuiHearts hearts;
		GuiCross cross;
		GuiBar bar;

	public:
		Gui();
		~Gui();
		void draw();
		static void init();
		static void updateMat();
		static inline glm::mat4 &getMat() {return (mat);};
		static inline Texture *getInventoryTex() {return (inventoryTex);};
		static inline Texture *getIconsTex() {return (iconsTex);};
		static inline Texture *getWaterTex() {return (waterTex);};
		static inline Texture *getLogoTex() {return (logoTex);};
		static inline Texture *getGuiTex() {return (guiTex);};
		static inline Texture *getBgTex() {return (bgTex);};
		static Color &getTextColor(char i);
		static Color &getTextShadowColor(char i);
	};

}

#endif
