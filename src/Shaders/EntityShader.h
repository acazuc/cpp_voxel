#ifndef ENTITY_SHADER_H
# define ENTITY_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class EntityShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation fogDistanceLocation;
		ProgramLocation fogDensityLocation;
		ProgramLocation vertexUVLocation;
		ProgramLocation fogColorLocation;
		ProgramLocation colorLocation;
		ProgramLocation mvpLocation;
		ProgramLocation texLocation;
		ProgramLocation mLocation;
		ProgramLocation vLocation;
		void load();

	};

}

#endif
