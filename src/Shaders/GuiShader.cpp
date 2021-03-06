#include "GuiShader.h"

namespace voxel
{

	void GuiShader::load()
	{
		loadProgram("data/shaders/gui.vs", "data/shaders/gui.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
	}

}
