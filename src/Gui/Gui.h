#ifndef GUI_H
# define GUI_H

# include <librender/Mat/Mat4.h>
# include <librender/Texture.h>

using librender::Texture;
using librender::Color;
using librender::Mat4;

namespace voxel
{

	class Gui
	{

	private:
		static Texture *inventoryTex;
		static Texture *iconsTex;
		static Texture *waterTex;
		static Texture *logoTex;
		static Texture *guiTex;
		static Texture *bgTex;
		static Mat4 mat;

	public:
		Gui();
		~Gui();
		void draw();
		static void init();
		static void clear();
		static void updateMat();
		static inline Texture *getInventoryTex() {return (inventoryTex);};
		static inline Texture *getIconsTex() {return (iconsTex);};
		static inline Texture *getWaterTex() {return (waterTex);};
		static inline Texture *getLogoTex() {return (logoTex);};
		static inline Texture *getGuiTex() {return (guiTex);};
		static inline Texture *getBgTex() {return (bgTex);};
		static inline Mat4 &getMat() {return (mat);};
		static Color &getTextColor(char i);
		static Color &getTextShadowColor(char i);
	};

}

#endif
