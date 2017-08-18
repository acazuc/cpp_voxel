#include "Main.h"
#include "Debug.h"
#include "World.h"
#include <cstring>
#include <glm/gtc/matrix_transform.hpp>
#include <librender/Shader/VertexShader.h>
#include <librender/Shader/FragmentShader.h>

using librender::VertexShader;
using librender::FragmentShader;

static char vShad[] = {"#version 120\n\
\n\
attribute vec3 vertexPosition;\n\
attribute vec3 vertexColor;\n\
attribute vec2 vertexUV;\n\
\n\
varying vec2 UV;\n\
varying vec3 color;\n\
\n\
uniform mat4 MVP;\n\
\n\
void main()\n\
{\n\
	gl_Position = MVP * vec4(vertexPosition, 1);\n\
	UV = vertexUV;\n\
	color = vertexColor;\n\
}\n\
"};

static char fShad[] = {"#version 120\n\
\n\
varying vec2 UV;\n\
varying vec3 color;\n\
\n\
uniform sampler2D tex;\n\
\n\
void main()\n\
{\n\
	vec4 texCol = texture2D(tex, UV);\n\
	gl_FragColor = vec4(texCol.xyz * color, texCol.w);\n\
}\n\
"};

namespace voxel
{

	ProgramLocation *Main::texCoordsLocation;
	ProgramLocation *Main::vertexesLocation;
	ProgramLocation *Main::colorsLocation;
	ProgramLocation *Main::mvpLocation;
	glm::mat4 Main::projMat;
	Program *Main::glProg;
	Window *Main::window;

	void Main::main()
	{
		glfwWindowHint(GLFW_SAMPLES, 8);
		window = new Window("C++ Voxel", 1280, 900);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			ERROR("GLAD failed");
		glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0, 0, 0, 1);
		window->show();
		window->setVSync(true);
		projMat = glm::perspective(glm::radians(45.), 1280. / 900., .1, 1000.);
		VertexShader *vertShad = new VertexShader(vShad);
		FragmentShader *fragShad = new FragmentShader(fShad);
		glProg = new Program(fragShad, vertShad);
		glProg->use();
		texCoordsLocation = glProg->getAttribLocation("vertexUV");
		texCoordsLocation->setVertexAttribArray(true);
		vertexesLocation = glProg->getAttribLocation("vertexPosition");
		vertexesLocation->setVertexAttribArray(true);
		colorsLocation = glProg->getAttribLocation("vertexColor");
		colorsLocation->setVertexAttribArray(true);
		mvpLocation = glProg->getUniformLocation("MVP");
		ProgramLocation *texLocation = glProg->getAttribLocation("tex");
		char *datas = new char[4];
		std::memset(datas, 0xff, 4);
		int width = 1;
		int height = 1;
		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, datas);
		delete[] (datas);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
		GLint osef = 0;
		texLocation->setVec1i(osef);
		World *world = new World();
		while (!window->closeRequested())
		{
			window->clearScreen();
			world->tick();
			glm::mat4 mvp = projMat * world->getPlayer().getViewMat();
			mvpLocation->setMat4f(mvp);
			world->draw();
			window->pollEvents();
			window->update();
		}
		delete (world);
		delete (window);
	}

}

int main()
{
	if (!glfwInit())
		ERROR("Failed to init glfw");
	voxel::Main::main();
	exit(EXIT_SUCCESS);
}
/*static void GLErrors(std::string err)
{
	GLenum glErr;
	while ((glErr = glGetError()) != GL_NO_ERROR)
	{
		std::cout << err << ": ";
		if (glErr == GL_INVALID_ENUM)
			std::cout << "GL_INVALID_ENUM" << std::endl;
		else if (glErr == GL_INVALID_VALUE)
			std::cout << "GL_INVALID_VALUE" << std::endl;
		else if (glErr == GL_INVALID_OPERATION)
			std::cout << "GL_INVALID_OPERATION" << std::endl;
		else if (glErr == GL_STACK_OVERFLOW)
			std::cout << "GL_STACK_OVERFLOW" << std::endl;
		else if (glErr == GL_STACK_UNDERFLOW)
			std::cout << "GL_STACK_UNDERFLOW" << std::endl;
		else if (glErr == GL_OUT_OF_MEMORY)
			std::cout << "GL_OUT_OF_MEMORY" << std::endl;
	}
}*/

