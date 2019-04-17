#include "GuiTextShader.h"

namespace voxel
{

	void GuiTextShader::load()
	{
		loadProgram("data/shaders/guitext.vs", "data/shaders/guitext.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
	}

}
