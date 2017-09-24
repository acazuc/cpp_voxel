#ifndef SKYBOX_SHADER_H
# define SKYBOX_SHADER_H

# include <librender/Shader/Program.h>

using librender::ProgramLocation;
using librender::FragmentShader;
using librender::VertexShader;
using librender::Program;

namespace voxel
{

	class SkyboxShader
	{
	
	public:
		ProgramLocation *vertexesLocation;
		ProgramLocation *colorsLocation;
		ProgramLocation *mvpLocation;
		FragmentShader *fShad;
		VertexShader *vShad;
		Program *program;
		SkyboxShader();
		~SkyboxShader();
		void load();

	};

}

#endif
