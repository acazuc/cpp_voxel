#ifndef DROPPED_SHADER_H
# define DROPPED_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::FragmentShader;
using librender::VertexShader;
using librender::Program;

namespace voxel
{

	class DroppedShader
	{

	public:
		ProgramLocation *fogDistanceLocation;
		ProgramLocation *texCoordsLocation;
		ProgramLocation *vertexesLocation;
		ProgramLocation *fogColorLocation;
		ProgramLocation *colorLocation;
		ProgramLocation *mvpLocation;
		ProgramLocation *texLocation;
		ProgramLocation *mLocation;
		ProgramLocation *vLocation;
		FragmentShader *fShad;
		VertexShader *vShad;
		Program *program;
		DroppedShader();
		~DroppedShader();
		void load();

	};

}

#endif
