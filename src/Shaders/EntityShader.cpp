#include "EntityShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <cstring>
#include <string>

namespace voxel
{

	EntityShader::EntityShader()
	{
		std::memset(this, 0, sizeof(*this));
	}

	EntityShader::~EntityShader()
	{
		delete (this->fogDistanceLocation);
		delete (this->texCoordsLocation);
		delete (this->vertexesLocation);
		delete (this->fogColorLocation);
		delete (this->colorLocation);
		delete (this->mvpLocation);
		delete (this->texLocation);
		delete (this->mLocation);
		delete (this->vLocation);
		delete (this->program);
		delete (this->vShad);
		delete (this->fShad);
	}

	void EntityShader::load()
	{
		std::string vertShad = readfile("data/shaders/entity.vs");
		LOG("building entity vertex shader");
		this->vShad = new VertexShader(vertShad.c_str());
		LOG("building entity fragment shader");
		std::string fragShad = readfile("data/shaders/entity.fs");
		this->fShad = new FragmentShader(fragShad.c_str());
		this->program = new Program();
		this->program->attachShader(this->vShad);
		this->program->attachShader(this->fShad);
		this->program->link();
		this->fogDistanceLocation = this->program->getUniformLocation("fogDistance");
		this->texCoordsLocation = this->program->getAttribLocation("vertexUV");
		this->texCoordsLocation->setVertexAttribArray(true);
		this->vertexesLocation = this->program->getAttribLocation("vertexPosition");
		this->vertexesLocation->setVertexAttribArray(true);
		this->fogColorLocation = this->program->getUniformLocation("fogColor");
		this->colorLocation = this->program->getUniformLocation("color");
		this->mvpLocation = this->program->getUniformLocation("MVP");
		this->texLocation = this->program->getAttribLocation("tex");
		this->mLocation = this->program->getUniformLocation("M");
		this->vLocation = this->program->getUniformLocation("V");
	}

}
