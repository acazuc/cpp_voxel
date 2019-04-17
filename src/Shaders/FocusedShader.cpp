#include "FocusedShader.h"

namespace voxel
{

	void FocusedShader::load()
	{
		loadProgram("data/shaders/focused.vs", "data/shaders/focused.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadUniform(this->mvpLocation, "MVP");
	}

}
