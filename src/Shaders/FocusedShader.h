#ifndef FOCUSED_SHADER_H
# define FOCUSED_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::FragmentShader;
using librender::VertexShader;
using librender::Program;

namespace voxel
{

	class FocusedShader
	{

	public:
		ProgramLocation *vertexesLocation;
		ProgramLocation *mvpLocation;
		FragmentShader *fShad;
		VertexShader *vShad;
		Program *program;
		FocusedShader();
		~FocusedShader();
		void load();

	};

}

#endif
