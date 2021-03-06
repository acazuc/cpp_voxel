#ifndef BREAK_SHADER_H
# define BREAK_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class BreakShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation fogDistanceLocation;
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
