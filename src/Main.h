#ifndef MAIN_H
# define MAIN_H

# include "Shaders/ParticlesShader.h"
# include "Shaders/FocusedShader.h"
# include "Shaders/SunMoonShader.h"
# include "Shaders/BlocksShader.h"
# include "Shaders/CloudsShader.h"
# include "Shaders/SkyboxShader.h"
# include "Shaders/EntityShader.h"
# include "Shaders/BreakShader.h"
# include "Shaders/GuiShader.h"
# include "Screen.h"
# include <librender/Window/Window.h>
# include <librender/Font/Font.h>
# include <librender/Texture.h>

using librender::KeyEvent;
using librender::Texture;
using librender::Window;
using librender::Font;

namespace voxel
{

	class Main
	{

	private:
		static ParticlesShader *particlesShader;
		static FocusedShader *focusedShader;
		static SunMoonShader *sunMoonShader;
		static BlocksShader *blocksShader;
		static CloudsShader *cloudsShader;
		static SkyboxShader *skyboxShader;
		static EntityShader *entityShader;
		static BreakShader *breakShader;
		static GuiShader *guiShader;
		static glm::vec4 skyColor;
		static Texture *terrain;
		static Window *window;
		static Screen *screen;
		static Font *font;
		static int64_t chunkUpdates;
		static int64_t fps;
		static bool smooth;
		static bool ssao;
		static int disableTex;
		static void keyDown(KeyEvent &event);

	public:
		static void main();
		static void glErrors(std::string str);
		static ParticlesShader &getParticlesShader() {return (*particlesShader);};
		static FocusedShader &getFocusedShader() {return (*focusedShader);};
		static SunMoonShader &getSunMoonShader() {return (*sunMoonShader);};
		static BlocksShader &getBlocksShader() {return (*blocksShader);};
		static CloudsShader &getCloudsShader() {return (*cloudsShader);};
		static SkyboxShader &getSkyboxShader() {return (*skyboxShader);};
		static EntityShader &getEntityShader() {return (*entityShader);};
		static BreakShader &getBreakShader() {return (*breakShader);};
		static GuiShader &getGuiShader() {return (*guiShader);};
		static glm::vec4 &getSkyColor() {return (skyColor);};
		static Texture *getTerrain() {return (terrain);};
		static Window *getWindow() {return (window);};
		static Screen *getScreen() {return (screen);};
		static Font *getFont() {return (font);};
		static void setChunkUpdates(int64_t chunkUpdates) {Main::chunkUpdates = chunkUpdates;};
		static int64_t getChunkUpdates() {return (chunkUpdates);};
		static int64_t getFps() {return (fps);};
		static bool getSmooth() {return (smooth);};
		static bool getSsao() {return (ssao);};

	};

}

#endif
