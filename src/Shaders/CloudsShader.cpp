#include "CloudsShader.h"

namespace voxel
{

	void CloudsShader::load()
	{
		loadProgram("data/shaders/clouds.vs", "data/shaders/clouds.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadUniform(this->fogDistanceLocation, "fogDistance");
		loadUniform(this->fogColorLocation, "fogColor");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->mLocation, "M");
		loadUniform(this->vLocation, "V");
	}

}
