#ifndef FOCUSED_SHADER_H
# define FOCUSED_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::Program;

namespace voxel
{

	class FocusedShader
	{

	public:
		ProgramLocation *fogDistanceLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *fogColorLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *mLocation;
		ProgramLocation *vLocation;
		Program *program;
		void load();

	};

}

#endif