#include "BreakShader.h"

namespace voxel
{

	void BreakShader::load()
	{
		loadProgram("data/shaders/break.vs", "data/shaders/break.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->fogDistanceLocation, "fogDistance");
		loadUniform(this->fogColorLocation, "fogColor");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
		loadUniform(this->mLocation, "M");
		loadUniform(this->vLocation, "V");
	}

}
