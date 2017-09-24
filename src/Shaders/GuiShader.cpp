#include "GuiShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <cstring>
#include <string>

namespace voxel
{

	GuiShader::GuiShader()
	{
		std::memset(this, 0, sizeof(*this));
	}

	GuiShader::~GuiShader()
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

	void GuiShader::load()
	{
		std::string vertShad = readfile("data/shaders/gui.vs");
		LOG("building particles vertex shader");
		this->vShad = new VertexShader(vertShad.c_str());
		LOG("building particles fragment shader");
		std::string fragShad = readfile("data/shaders/gui.fs");
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
