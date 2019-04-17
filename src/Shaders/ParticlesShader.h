#ifndef PARTICLES_SHADER_H
# define PARTICLES_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class ParticlesShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation fogDistanceLocation;
		ProgramLocation vertexColorLocation;
		ProgramLocation vertexUVLocation;
		ProgramLocation fogColorLocation;
		ProgramLocation mvpLocation;
		ProgramLocation texLocation;
		ProgramLocation mLocation;
		ProgramLocation vLocation;
		void load();

	};

}

#endif
