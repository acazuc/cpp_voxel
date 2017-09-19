#include "GuiShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <librender/Shader/FragmentShader.h>
#include <librender/Shader/VertexShader.h>
#include <string>
#include "Main.h"

using librender::FragmentShader;
using librender::VertexShader;

namespace voxel
{

	void GuiShader::load()
	{
		std::string vShad = readfile("data/shaders/gui.vs");
		LOG("building particles vertex shader");
		VertexShader *vertShad = new VertexShader(vShad.c_str());
		LOG("building particles fragment shader");
		std::string fShad = readfile("data/shaders/gui.fs");
		FragmentShader *fragShad = new FragmentShader(fShad.c_str());
		this->program = new Program();
		this->program->attachShader(vertShad);
		this->program->attachShader(fragShad);
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
