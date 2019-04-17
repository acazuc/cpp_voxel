#ifndef SKYBOX_SHADER_H
# define SKYBOX_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class SkyboxShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation vertexColorLocation;
		ProgramLocation mvpLocation;
		void load();

	};

}

#endif
