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
	gl_Position =  MVP * vec4(vertexPosition, 1);\n\
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
		window = new Window("Map generator", 1280, 900);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			ERROR("GLAD failed");
		glEnable(GL_BLEND);
		glClearColor(0, 0, 0, 1);
		window->show();
		window->setVSync(true);
		projMat = glm::perspective(glm::radians(45.), 1280. / 900., .1, 1000.);
		VertexShader *vertShad = new VertexShader(vShad);
		FragmentShader *fragShad = new FragmentShader(fShad);
		glProg = new Program(fragShad, vertShad);
		glProg->use();
		ProgramLocation texCoordsLocation = glProg->getAttribLocation("vertexUV");
		ProgramLocation vertexesLocation = glProg->getAttribLocation("vertexPosition");
		ProgramLocation colorsLocation = glProg->getAttribLocation("vertexColor");
		ProgramLocation mvpLocation = glProg->getAttribLocation("MVP");
		ProgramLocation texLocation = glProg->getAttribLocation("tex");
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
		texLocation.setVec1i(osef);
		Main::texCoordsLocation = &texCoordsLocation;
		Main::vertexesLocation = &vertexesLocation;
		Main::colorsLocation = &colorsLocation;
		Main::mvpLocation = &mvpLocation;
		World *world = new World();
		while (!window->closeRequested())
		{
			world->tick();
			world->draw();
			window->pollEvents();
			window->update();
		}
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
