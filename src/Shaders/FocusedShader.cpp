#include "FocusedShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <librender/Shader/FragmentShader.h>
#include <librender/Shader/VertexShader.h>
#include <string>

using librender::FragmentShader;
using librender::VertexShader;

namespace voxel
{

	void FocusedShader::load()
	{
		std::string vShad = readfile("data/shaders/focused.vs");
		LOG("building focused vertex shader");
		VertexShader *vertShad = new VertexShader(vShad.c_str());
		std::string fShad = readfile("data/shaders/focused.fs");
		LOG("building focused fragment shader");
		FragmentShader *fragShad = new FragmentShader(fShad.c_str());
		this->program = new Program();
		this->program->attachShader(vertShad);
		this->program->attachShader(fragShad);
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
