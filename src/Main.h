#ifndef MAIN_H
# define MAIN_H

# include "Shaders/BlocksShader.h"
# include "Shaders/CloudsShader.h"
# include "Shaders/SkyboxShader.h"
# include <librender/Shader/ProgramLocation.h>
# include <librender/Shader/Program.h>
# include <librender/Window/Window.h>
# include <librender/Texture.h>

using librender::ProgramLocation;
using librender::Program;
using librender::Texture;
using librender::Window;
using librender::Color;

namespace voxel
{

	class Main
	{

	private:
		static BlocksShader blocksShader;
		static CloudsShader cloudsShader;
		static SkyboxShader skyboxShader;
		static glm::vec3 skyColor;
		static Texture *terrain;
		static Window *window;
		static void buildBlocksShader();
		static void buildCloudsShader();
		static void buildSkyboxShader();

	public:
		static void main();
		static BlocksShader &getBlocksShader() {return (blocksShader);};
		static CloudsShader &getCloudsShader() {return (cloudsShader);};
		static SkyboxShader &getSkyboxShader() {return (skyboxShader);};
		static glm::vec3 &getSkyColor() {return (skyColor);};
		static Texture *getTerrain() {return (terrain);};
		static Window *getWindow() {return (window);};

	};

}

#endif
