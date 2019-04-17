#include "EntityShader.h"

namespace voxel
{

	void EntityShader::load()
	{
		loadProgram("data/shaders/entity.vs", "data/shaders/entity.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->fogDistanceLocation, "fogDistance");
		loadUniform(this->fogDensityLocation, "fogDensity");
		loadUniform(this->fogColorLocation, "fogColor");
		loadUniform(this->colorLocation, "color");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
		loadUniform(this->mLocation, "M");
		loadUniform(this->vLocation, "V");
	}

}
