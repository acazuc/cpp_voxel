#include "ParticlesShader.h"

namespace voxel
{

	void ParticlesShader::load()
	{
		loadProgram("data/shaders/particles.vs", "data/shaders/particles.fs");
		loadAttrib(this->vertexPositionLocation, "vertexPosition");
		loadAttrib(this->vertexColorLocation, "vertexColor");
		loadAttrib(this->vertexUVLocation, "vertexUV");
		loadUniform(this->fogDistanceLocation, "fogDistance");
		loadUniform(this->fogColorLocation, "fogColor");
		loadUniform(this->mvpLocation, "MVP");
		loadUniform(this->texLocation, "tex");
		loadUniform(this->mLocation, "M");
		loadUniform(this->vLocation, "V");
	}

}
