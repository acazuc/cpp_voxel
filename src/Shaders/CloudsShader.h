#ifndef CLOUDS_SHADER_H
# define CLOUDS_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class CloudsShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation fogDistanceLocation;
		ProgramLocation vertexColorLocation;
		ProgramLocation fogColorLocation;
		ProgramLocation mvpLocation;
		ProgramLocation mLocation;
		ProgramLocation vLocation;
		void load();

	};

}

#endif
