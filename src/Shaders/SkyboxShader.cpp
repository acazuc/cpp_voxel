#include "SkyboxShader.h"

namespace voxel
{

	void SkyboxShader::load()
	{
		loadProgram("data/shaders/skybox.vs", "data/shaders/skybox.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadUniform(this->mvpLocation, "MVP");
	}

}
