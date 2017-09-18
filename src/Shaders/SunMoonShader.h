#ifndef SUN_MOON_SHADER_H
# define SUN_MOON_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::Program;

namespace voxel
{

	class SunMoonShader
	{
	
	public:
		ProgramLocation *texCoordsLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *texLocation;
		Program *program;
		void load();

	};

}

#endif
