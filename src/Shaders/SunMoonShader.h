#ifndef SUN_MOON_SHADER_H
# define SUN_MOON_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class SunMoonShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation vertexColorLocation;
		ProgramLocation vertexUVLocation;
		ProgramLocation mvpLocation;
		ProgramLocation texLocation;
		void load();

	};

}

#endif
