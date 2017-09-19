#include "BreakShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <librender/Shader/FragmentShader.h>
#include <librender/Shader/VertexShader.h>
#include <string>

using librender::FragmentShader;
using librender::VertexShader;

namespace voxel
{

	void BreakShader::load()
	{
		std::string vShad = readfile("data/shaders/break.vs");
		LOG("building blocks vertex shader");
		VertexShader *vertShad = new VertexShader(vShad.c_str());
		LOG("building blocks fragment shader");
		std::string fShad = readfile("data/shaders/break.fs");
		FragmentShader *fragShad = new FragmentShader(fShad.c_str());
		this->program = new Program();
		this->program->attachShader(vertShad);
		this->program->attachShader(fragShad);
		this->program->link();
		this->fogDistanceLocation = this->program->getUniformLocation("fogDistance");
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