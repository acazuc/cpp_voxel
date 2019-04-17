#ifndef FOCUSED_SHADER_H
# define FOCUSED_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class FocusedShader : public Shader
	{

	public:
		ProgramLocation vertexPositionLocation;
		ProgramLocation mvpLocation;
		void load();

	};

}

#endif
