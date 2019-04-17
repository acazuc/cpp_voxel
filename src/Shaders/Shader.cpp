#include "Shader.h"
#include "Utils/readfile.h"

using librender::VertexShader;
using librender::FragmentShader;

namespace voxel
{

	Shader::Shader()
	: program(nullptr)
	{
	}

	Shader::~Shader()
	{
		delete (this->program);
	}

	void Shader::loadProgram(std::string vertex, std::string fragment)
	{
		std::string vertShad = readfile(vertex);
		VertexShader *vShad = new VertexShader(vertShad.c_str());
		std::string fragShad = readfile(fragment);
		FragmentShader *fShad = new FragmentShader(fragShad.c_str());
		this->program = new Program();
		this->program->attachShader(vShad);
		this->program->attachShader(fShad);
		this->program->link();
		delete (vShad);
		delete (fShad);
	}

	void Shader::loadUniform(ProgramLocation &location, std::string name)
	{
		location = this->program->getUniformLocation(name.c_str());
	}

	void Shader::loadAttrib(ProgramLocation &location, std::string name)
	{
		location = this->program->getAttribLocation(name.c_str());
		location.setVertexAttribArray(true);
	}

}
