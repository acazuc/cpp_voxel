#include "FocusedShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <cstring>
#include <string>

namespace voxel
{

	FocusedShader::FocusedShader()
	{
		std::memset(this, 0, sizeof(*this));
	}

	FocusedShader::~FocusedShader()
	{
		delete (this->vertexesLocation);
		delete (this->mvpLocation);
		delete (this->program);
		delete (this->vShad);
		delete (this->fShad);
	}

	void FocusedShader::load()
	{
		std::string vertShad = readfile("data/shaders/focused.vs");
		LOG("building focused vertex shader");
		this->vShad = new VertexShader(vertShad.c_str());
		std::string fragShad = readfile("data/shaders/focused.fs");
		LOG("building focused fragment shader");
		this->fShad = new FragmentShader(fragShad.c_str());
		this->program = new Program();
		this->program->attachShader(this->vShad);
		this->program->attachShader(this->fShad);
		this->program->link();
		this->vertexesLocation = this->program->getAttribLocation("vertexPosition");
		this->vertexesLocation->setVertexAttribArray(true);
		this->mvpLocation = this->program->getUniformLocation("MVP");
	}

}
