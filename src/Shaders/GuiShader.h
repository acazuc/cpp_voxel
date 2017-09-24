#ifndef GUI_SHADER_H
# define GUI_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::FragmentShader;
using librender::VertexShader;
using librender::Program;

namespace voxel
{

	class GuiShader
	{

	public:
		ProgramLocation *texCoordsLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *texLocation;
		FragmentShader *fShad;
		VertexShader *vShad;
		Program *program;
		GuiShader();
		~GuiShader();
		void load();

	};

}

#endif
