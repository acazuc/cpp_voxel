#ifndef MAIN_H
# define MAIN_H

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
		static ProgramLocation *texCoordsLocation;
		static ProgramLocation *vertexesLocation;
		static ProgramLocation *colorsLocation;
		static ProgramLocation *mvpLocation;
		static ProgramLocation *mLocation;
		static ProgramLocation *vLocation;
		static Texture *terrain;
		static Program *glProg;
		static Window *window;

	public:
		static void main();
		static ProgramLocation *getTexCoordsLocation() {return (texCoordsLocation);};
		static ProgramLocation *getVertexesLocation() {return (vertexesLocation);};
		static ProgramLocation *getColorsLocation() {return (colorsLocation);};
		static ProgramLocation *getMvpLocation() {return (mvpLocation);};
		static ProgramLocation *getMLocation() {return (mLocation);};
		static ProgramLocation *getVLocation() {return (vLocation);};
		static Window *getWindow() {return (window);};

	};

}

#endif
