#ifndef GUI_TEXT_SHADER_H
# define GUI_TEXT_SHADER_H

# include "./Shader.h"

namespace voxel
{

	class GuiTextShader : public Shader
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
