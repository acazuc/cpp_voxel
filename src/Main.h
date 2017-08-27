#ifndef MAIN_H
# define MAIN_H

# include "Shaders/BlocksShader.h"
# include "Shaders/CloudsShader.h"
# include <librender/Shader/ProgramLocation.h>
# include <librender/Shader/Program.h>
# include <librender/Window/Window.h>
# include <librender/Texture.h>

using librender::ProgramLocation;
using librender::Program;
using librender::Texture;
using librender::Window;

namespace voxel
{

	class Main
	{

	private:
		static BlocksShader blocksShader;
		static CloudsShader cloudsShader;
		static Texture *terrain;
		static Window *window;
		static void buildBlocksShader();
		static void buildCloudsShader();

	public:
		static void main();
		static BlocksShader &getBlocksShader() {return (blocksShader);};
		static CloudsShader &getCloudsShader() {return (cloudsShader);};
		static Window *getWindow() {return (window);};

	};

}

#endif
