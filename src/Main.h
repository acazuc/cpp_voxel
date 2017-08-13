#ifndef MAIN_H
# define MAIN_H

# include <librender/Shader/ProgramLocation.h>
# include <librender/Shader/Program.h>
# include <librender/Window/Window.h>

using librender::ProgramLocation;
using librender::Program;
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
		static glm::mat4 projMat;
		static Program *glProg;
		static Window *window;

	public:
		static void main();
		static ProgramLocation *getTexCoordsLocation() {return (texCoordsLocation);};
		static ProgramLocation *getVertexesLocation() {return (vertexesLocation);};
		static ProgramLocation *getColorsLocation() {return (colorsLocation);};
		static ProgramLocation *getMvpLocation() {return (mvpLocation);};
		static glm::mat4 &getProjMat() {return (projMat);};
		static Window *getWindow() {return (window);};

	};

}

#endif
