#ifndef BREAK_SHADER_H
# define BREAK_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::FragmentShader;
using librender::VertexShader;
using librender::Program;

namespace voxel
{

	class BreakShader
	{

	public:
		ProgramLocation *fogDistanceLocation;
		ProgramLocation *texCoordsLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *fogColorLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *texLocation;
		ProgramLocation *mLocation;
		ProgramLocation *vLocation;
		FragmentShader *fShad;
		VertexShader *vShad;
		Program *program;
		BreakShader();
		~BreakShader();
		void load();

	};

}

#endif
