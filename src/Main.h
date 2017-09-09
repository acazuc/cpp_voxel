#ifndef MAIN_H
# define MAIN_H

# include "Shaders/ParticlesShader.h"
# include "Shaders/FocusedShader.h"
# include "Shaders/BlocksShader.h"
# include "Shaders/CloudsShader.h"
# include "Shaders/SkyboxShader.h"
# include "Shaders/EntityShader.h"
# include "World.h"
# include <librender/Window/Window.h>
# include <librender/Texture.h>

using librender::KeyEvent;
using librender::Texture;
using librender::Window;
using librender::Color;

namespace voxel
{

	class Main
	{

	private:
		static ParticlesShader particlesShader;
		static FocusedShader focusedShader;
		static BlocksShader blocksShader;
		static CloudsShader cloudsShader;
		static SkyboxShader skyboxShader;
		static EntityShader entityShader;
		static glm::vec4 skyColor;
		static Texture *terrain;
		static Window *window;
		static World *world;
		static bool smooth;
		static bool ssao;
		static int disableTex;
		static void keyDown(KeyEvent &event);

	public:
		static void main();
		static void glErrors(std::string str);
		static ParticlesShader &getParticlesShader() {return (particlesShader);};
		static FocusedShader &getFocusedShader() {return (focusedShader);};
		static BlocksShader &getBlocksShader() {return (blocksShader);};
		static CloudsShader &getCloudsShader() {return (cloudsShader);};
		static SkyboxShader &getSkyboxShader() {return (skyboxShader);};
		static EntityShader &getEntityShader() {return (entityShader);};
		static glm::vec4 &getSkyColor() {return (skyColor);};
		static Texture *getTerrain() {return (terrain);};
		static Window *getWindow() {return (window);};
		static bool getSmooth() {return (smooth);};
		static bool getSsao() {return (ssao);};

	};

}

#endif
