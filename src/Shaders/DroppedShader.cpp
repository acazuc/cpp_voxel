#include "DroppedShader.h"

namespace voxel
{

	void DroppedShader::load()
	{
		loadProgram("data/shaders/dropped.vs", "data/shaders/dropped.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->fogDistanceLocation, "fogDistance");
		loadUniform(this->fogDensityLocation, "fogDensity");
		loadUniform(this->fogColorLocation, "fogColor");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
		loadUniform(this->mLocation, "M");
		loadUniform(this->vLocation, "V");
	}

}
