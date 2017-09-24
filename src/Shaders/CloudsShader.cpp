#include "CloudsShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <cstring>
#include <string>

namespace voxel
{

	CloudsShader::CloudsShader()
	{
		std::memset(this, 0, sizeof(*this));
	}

	CloudsShader::~CloudsShader()
	{
		delete (this->fogDistanceLocation);
		delete (this->vertexesLocation);
		delete (this->fogColorLocation);
		delete (this->colorsLocation);
		delete (this->mvpLocation);
		delete (this->mLocation);
		delete (this->vLocation);
		delete (this->program);
		delete (this->vShad);
		delete (this->fShad);
	}

	void CloudsShader::load()
	{
		std::string vertShad = readfile("data/shaders/clouds.vs");
		LOG("building clouds vertex shader");
		this->vShad = new VertexShader(vertShad.c_str());
		std::string fragShad = readfile("data/shaders/clouds.fs");
		LOG("building clouds fragment shader");
		this->fShad = new FragmentShader(fragShad.c_str());
		this->program = new Program();
		this->program->attachShader(this->vShad);
		this->program->attachShader(this->fShad);
		this->program->link();
		this->fogDistanceLocation = this->program->getUniformLocation("fogDistance");
		this->vertexesLocation = this->program->getAttribLocation("vertexPosition");
		this->vertexesLocation->setVertexAttribArray(true);
		this->fogColorLocation = this->program->getUniformLocation("fogColor");
		this->colorsLocation = this->program->getAttribLocation("vertexColor");
		this->colorsLocation->setVertexAttribArray(true);
		this->mvpLocation = this->program->getUniformLocation("MVP");
		this->mLocation = this->program->getUniformLocation("M");
		this->vLocation = this->program->getUniformLocation("V");
	}

}
