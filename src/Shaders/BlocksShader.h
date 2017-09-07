#ifndef BLOCKS_SHADER_H
# define BLOCKS_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::Program;

namespace voxel
{

	class BlocksShader
	{

	public:
		ProgramLocation *fogDistanceLocation;
		ProgramLocation *timeFactorLocation;
		ProgramLocation *disableTexLocation;
		ProgramLocation *texCoordsLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *fogColorLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *texLocation;
		ProgramLocation *mLocation;
		ProgramLocation *vLocation;
		Program *program;
		void load();

	};

}

#endif
