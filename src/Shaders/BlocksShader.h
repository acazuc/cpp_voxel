#ifndef BLOCKS_SHADER_H
# define BLOCKS_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::FragmentShader;
using librender::VertexShader;
using librender::Program;

namespace voxel
{

	class BlocksShader
	{

	public:
		ProgramLocation *fogDistanceLocation;
		ProgramLocation *timeFactorLocation;
		ProgramLocation *disableTexLocation;
		ProgramLocation *fogDensityLocation;
		ProgramLocation *texCoordsLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *fogColorLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *texLocation;
		ProgramLocation *mLocation;
		ProgramLocation *vLocation;
		FragmentShader *fShad;
		VertexShader *vShad;
		Program *program;
		BlocksShader();
		~BlocksShader();
		void load();

	};

}

#endif
