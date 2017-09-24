#include "SunMoonShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <cstring>
#include <string>

using librender::FragmentShader;
using librender::VertexShader;

namespace voxel
{

	SunMoonShader::SunMoonShader()
	{
		std::memset(this, 0, sizeof(*this));
	}

	SunMoonShader::~SunMoonShader()
	{
		delete (this->texCoordsLocation);
		delete (this->vertexesLocation);
		delete (this->colorsLocation);
		delete (this->mvpLocation);
		delete (this->texLocation);
		delete (this->program);
		delete (this->vShad);
		delete (this->fShad);
	}

	void SunMoonShader::load()
	{
		std::string vertShad = readfile("data/shaders/sun_moon.vs");
		LOG("building skybox vertex shader");
		this->vShad = new VertexShader(vertShad.c_str());
		LOG("building skybox fragment shader");
		std::string fragShad = readfile("data/shaders/sun_moon.fs");
		this->fShad = new FragmentShader(fragShad.c_str());
		this->program = new Program();
		this->program->attachShader(this->vShad);
		this->program->attachShader(this->fShad);
		this->program->link();
		this->texCoordsLocation = this->program->getAttribLocation("vertexUV");
		this->texCoordsLocation->setVertexAttribArray(true);
		this->vertexesLocation = this->program->getAttribLocation("vertexPosition");
		this->vertexesLocation->setVertexAttribArray(true);
		this->colorsLocation = this->program->getAttribLocation("vertexColor");
		this->colorsLocation->setVertexAttribArray(true);
		this->mvpLocation = this->program->getUniformLocation("MVP");
		this->texLocation = this->program->getAttribLocation("tex");
	}

}
