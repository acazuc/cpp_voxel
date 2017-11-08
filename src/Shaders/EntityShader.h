#ifndef ENTITY_SHADER_H
# define ENTITY_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::FragmentShader;
using librender::VertexShader;
using librender::Program;

namespace voxel
{

	class EntityShader
	{

	public:
		ProgramLocation *fogDistanceLocation;
		ProgramLocation *fogDensityLocation;
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
		EntityShader();
		~EntityShader();
		void load();

	};

}

#endif
