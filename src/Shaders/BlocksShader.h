#ifndef BLOCKS_SHADER_H
# define BLOCKS_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class BlocksShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation fogDistanceLocation;
		ProgramLocation vertexColorLocation;
		ProgramLocation timeFactorLocation;
		ProgramLocation disableTexLocation;
		ProgramLocation fogDensityLocation;
		ProgramLocation fogColorLocation;
		ProgramLocation vertexUVLocation;
		ProgramLocation mvpLocation;
		ProgramLocation texLocation;
		ProgramLocation mLocation;
		ProgramLocation vLocation;
		void load();

	};

}

#endif
