#ifndef MAIN_H
# define MAIN_H

# include "Shaders/ParticlesShader.h"
# include "Shaders/FocusedShader.h"
# include "Shaders/SunMoonShader.h"
# include "Shaders/DroppedShader.h"
# include "Shaders/GuiTextShader.h"
# include "Shaders/BlocksShader.h"
# include "Shaders/CloudsShader.h"
# include "Shaders/SkyboxShader.h"
# include "Shaders/EntityShader.h"
# include "Shaders/BreakShader.h"
# include "Shaders/GuiShader.h"
# include "Screen.h"
# include <librender/Texture/Texture.h>
# include <librender/Window/Window.h>
# include <librender/Font/Font.h>

using librender::ScrollEvent;
using librender::KeyEvent;
using librender::Texture;
using librender::Window;
using librender::Font;
using librender::Vec4;

namespace voxel
{

	class Main
	{

	private:
		static ParticlesShader *particlesShader;
		static FocusedShader *focusedShader;
		static SunMoonShader *sunMoonShader;
		static DroppedShader *droppedShader;
		static GuiTextShader *guiTextShader;
		static BlocksShader *blocksShader;
		static CloudsShader *cloudsShader;
		static SkyboxShader *skyboxShader;
		static EntityShader *entityShader;
		static BreakShader *breakShader;
		static GuiShader *guiShader;
		static Texture *unknownPack;
		static Texture *terrain;
		static Texture *empty;
		static Window *window;
		static Screen *screen;
		static Font *font;
		static Vec4 skyColor;
		static int64_t chunkUpdates;
		static uint8_t guiScale;
		static int64_t fps;
		static bool smooth;
		static bool ssao;
		static int disableTex;
		static void mouseMove();
		static void mouseDown(MouseEvent &event);
		static void mouseUp(MouseEvent &event);
		static void mouseScroll(ScrollEvent &event);
		static void keyDown(KeyEvent &event);

	public:
		static void main(int ac, char **av);
		static void glErrors(std::string str);
		static ParticlesShader &getParticlesShader() {return *particlesShader;};
		static FocusedShader &getFocusedShader() {return *focusedShader;};
		static SunMoonShader &getSunMoonShader() {return *sunMoonShader;};
		static DroppedShader &getDroppedShader() {return *droppedShader;};
		static GuiTextShader &getGuiTextShader() {return *guiTextShader;};
		static BlocksShader &getBlocksShader() {return *blocksShader;};
		static CloudsShader &getCloudsShader() {return *cloudsShader;};
		static SkyboxShader &getSkyboxShader() {return *skyboxShader;};
		static EntityShader &getEntityShader() {return *entityShader;};
		static BreakShader &getBreakShader() {return *breakShader;};
		static GuiShader &getGuiShader() {return *guiShader;};
		static Texture *getUnknownPack() {return unknownPack;};
		static Texture *getTerrain() {return terrain;};
		static Texture *getEmpty() {return empty;};
		static Window *getWindow() {return window;};
		static Screen *getScreen() {return screen;};
		static Font *getFont() {return font;};
		static Vec4 &getSkyColor() {return skyColor;};
		static void setChunkUpdates(int64_t chunkUpdates) {Main::chunkUpdates = chunkUpdates;};
		static int64_t getChunkUpdates() {return chunkUpdates;};
		static uint8_t getGuiScale() {return guiScale;};
		static int64_t getFps() {return fps;};
		static bool getSmooth() {return smooth;};
		static bool getSsao() {return ssao;};

	};

}

#endif
