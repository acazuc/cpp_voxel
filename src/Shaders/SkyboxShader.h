#ifndef SKYBOX_SHADER_H
# define SKYBOX_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::Program;

namespace voxel
{

	struct SkyboxShader
	{
		ProgramLocation *texCoordsLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *texLocation;
		Program *program;
	};

}

#endif
