#include "SkyboxShader.h"
#include "Utils/readfile.h"
#include "Debug.h"
#include <librender/Shader/FragmentShader.h>
#include <librender/Shader/VertexShader.h>
#include <string>

using librender::FragmentShader;
using librender::VertexShader;

namespace voxel
{

	void SkyboxShader::load()
	{
		std::string vShad = readfile("data/shaders/skybox.vs");
		LOG("building skybox vertex shader");
		VertexShader *vertShad = new VertexShader(vShad.c_str());
		LOG("building skybox fragment shader");
		std::string fShad = readfile("data/shaders/skybox.fs");
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
