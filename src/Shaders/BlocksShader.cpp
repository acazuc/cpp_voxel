#include "BlocksShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <cstring>
#include <string>

namespace voxel
{

	BlocksShader::BlocksShader()
	{
		std::memset(this, 0, sizeof(*this));
	}

	BlocksShader::~BlocksShader()
	{
		delete (this->fogDistanceLocation);
		delete (this->timeFactorLocation);
		delete (this->disableTexLocation);
		delete (this->fogDensityLocation);
		delete (this->texCoordsLocation);
		delete (this->vertexesLocation);
		delete (this->fogColorLocation);
		delete (this->colorsLocation);
		delete (this->mvpLocation);
		delete (this->texLocation);
		delete (this->mLocation);
		delete (this->vLocation);
		delete (this->program);
		delete (this->fShad);
		delete (this->vShad);
	}

	void BlocksShader::load()
	{
		std::string vertShad = readfile("data/shaders/blocks.vs");
		LOG("building blocks vertex shader");
		this->vShad = new VertexShader(vertShad.c_str());
		LOG("building blocks fragment shader");
		std::string fragShad = readfile("data/shaders/blocks.fs");
		this->fShad = new FragmentShader(fragShad.c_str());
		this->program = new Program();
		this->program->attachShader(this->vShad);
		this->program->attachShader(this->fShad);
		this->program->link();
		this->fogDistanceLocation = this->program->getUniformLocation("fogDistance");
		this->timeFactorLocation = this->program->getUniformLocation("timeFactor");
		this->disableTexLocation = this->program->getUniformLocation("disableTex");
		this->fogDensityLocation = this->program->getUniformLocation("fogDensity");
		this->texCoordsLocation = this->program->getAttribLocation("vertexUV");
		this->texCoordsLocation->setVertexAttribArray(true);
		this->vertexesLocation = this->program->getAttribLocation("vertexPosition");
		this->vertexesLocation->setVertexAttribArray(true);
		this->fogColorLocation = this->program->getUniformLocation("fogColor");
		this->colorsLocation = this->program->getAttribLocation("vertexColor");
		this->colorsLocation->setVertexAttribArray(true);
		this->mvpLocation = this->program->getUniformLocation("MVP");
		this->texLocation = this->program->getAttribLocation("tex");
		this->mLocation = this->program->getUniformLocation("M");
		this->vLocation = this->program->getUniformLocation("V");
	}

}
