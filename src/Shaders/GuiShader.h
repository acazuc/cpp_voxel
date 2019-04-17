#ifndef GUI_SHADER_H
# define GUI_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class GuiShader : public Shader
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
