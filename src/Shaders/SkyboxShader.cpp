#include "SkyboxShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <cstring>
#include <string>

using librender::FragmentShader;
using librender::VertexShader;

namespace voxel
{

	SkyboxShader::SkyboxShader()
	{
		std::memset(this, 0, sizeof(*this));
	}

	SkyboxShader::~SkyboxShader()
	{
		delete (this->vertexesLocation);
		delete (this->colorsLocation);
		delete (this->mvpLocation);
		delete (this->program);
		delete (this->vShad);
		delete (this->fShad);
	}

	void SkyboxShader::load()
	{
		std::string vertShad = readfile("data/shaders/skybox.vs");
		LOG("building skybox vertex shader");
		this->vShad = new VertexShader(vertShad.c_str());
		LOG("building skybox fragment shader");
		std::string fragShad = readfile("data/shaders/skybox.fs");
		this->fShad = new FragmentShader(fragShad.c_str());
		this->program = new Program();
		this->program->attachShader(this->vShad);
		this->program->attachShader(this->fShad);
		this->program->link();
		this->vertexesLocation = this->program->getAttribLocation("vertexPosition");
		this->vertexesLocation->setVertexAttribArray(true);
		this->colorsLocation = this->program->getAttribLocation("vertexColor");
		this->colorsLocation->setVertexAttribArray(true);
		this->mvpLocation = this->program->getUniformLocation("MVP");
	}

}
