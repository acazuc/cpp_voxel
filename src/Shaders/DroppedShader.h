#ifndef DROPPED_SHADER_H
# define DROPPED_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class DroppedShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation fogDistanceLocation;
		ProgramLocation vertexColorLocation;
		ProgramLocation fogDensityLocation;
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
