#ifndef SKYBOX_SHADER_H
# define SKYBOX_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::Program;

namespace voxel
{

	class SkyboxShader
	{
	
	public:
		ProgramLocation *vertexesLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		Program *program;
		void load();

	};

}

#endif
