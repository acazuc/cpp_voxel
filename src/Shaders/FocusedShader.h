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
		ProgramLocation *vertexesLocation;
		ProgramLocation *mvpLocation;
		Program *program;
		void load();

	};

}

#endif
