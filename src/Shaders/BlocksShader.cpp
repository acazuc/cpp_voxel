#include "BlocksShader.h"

namespace voxel
{

	void BlocksShader::load()
	{
		loadProgram("data/shaders/blocks.vs", "data/shaders/blocks.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->fogDistanceLocation, "fogDistance");
		loadUniform(this->timeFactorLocation, "timeFactor");
		loadUniform(this->disableTexLocation, "disableTex");
		loadUniform(this->fogDensityLocation, "fogDensity");
		loadUniform(this->fogColorLocation, "fogColor");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
		loadUniform(this->mLocation, "M");
		loadUniform(this->vLocation, "V");
	}

}
