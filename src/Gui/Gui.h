#ifndef GUI_H
# define GUI_H

# include <librender/Texture.h>
# include <glm/mat4x4.hpp>

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

	public:
		Gui();
		~Gui();
		void draw();
		static void init();
		static void clear();
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
