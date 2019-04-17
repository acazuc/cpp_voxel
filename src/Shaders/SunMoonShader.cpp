#include "SunMoonShader.h"

namespace voxel
{

	void SunMoonShader::load()
	{
		loadProgram("data/shaders/sun_moon.vs", "data/shaders/sun_moon.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
	}

}
