#include "Main.h"
#include "Utils/System.h"
#include "Debug.h"
#include "World.h"
#include <cstring>
#include <glm/gtc/matrix_transform.hpp>
#include <librender/Shader/VertexShader.h>
#include <librender/Shader/FragmentShader.h>
#include <libformat/PNG.h>

using librender::VertexShader;
using librender::FragmentShader;

static char vShad[] = {"#version 120\n\
\n\
attribute vec3 vertexPosition;\n\
attribute vec3 vertexColor;\n\
attribute vec2 vertexUV;\n\
\n\
centroid varying vec2 UV;\n\
centroid varying vec3 color;\n\
centroid varying vec4 viewSpace;\n\
\n\
uniform mat4 MVP;\n\
uniform mat4 M;\n\
uniform mat4 V;\n\
uniform float timeFactor;\n\
\n\
void main()\n\
{\n\
	vec3 newVertex = vertexPosition;\n\
	//newVertex.x += cos(newVertex.x + newVertex.y + newVertex.z + timeFactor * 3.14) * .1;\n\
	//newVertex.y += cos(newVertex.x + newVertex.y + newVertex.z + timeFactor * 3.14 / 3) * .033;\n\
	//newVertex.z += cos(newVertex.x + newVertex.y + newVertex.z + timeFactor * 3.14 / 2) * .05;\n\
	gl_Position = MVP * vec4(newVertex, 1);\n\
	UV = vertexUV;\n\
	color = vertexColor;\n\
	viewSpace = M * V * vec4(newVertex, 1);\n\
}\n\
"};

static char fShad[] = {"#version 120\n\
\n\
centroid varying vec2 UV;\n\
centroid varying vec3 color;\n\
centroid varying vec4 viewSpace;\n\
\n\
uniform sampler2D tex;\n\
uniform float fogDistance;\n\
\n\
vec4 dayFogColor = vec4(.5, .6, .7, 1);\n\
vec4 nightFogColor = vec4(.01, .02, .025, 1);\n\
\n\
void main()\n\
{\n\
	vec4 texCol = texture2D(tex, UV);\n\
	vec4 col = texCol * vec4(color, 1);\n\
	float dist = length(viewSpace);\n\
	float fog = clamp(1 / exp(pow(max(0, dist - fogDistance), 2) * 0.01), 0, 1);\n\
	gl_FragColor = mix(col, dayFogColor, 1 - fog);\n\
}\n\
"};

int64_t frameDelta;
int64_t nanotime;

namespace voxel
{

	ProgramLocation *Main::fogDistanceLocation;
	ProgramLocation *Main::texCoordsLocation;
	ProgramLocation *Main::vertexesLocation;
	ProgramLocation *Main::colorsLocation;
	ProgramLocation *Main::mvpLocation;
	ProgramLocation *Main::mLocation;
	ProgramLocation *Main::vLocation;
	Texture *Main::terrain;
	Program *Main::glProg;
	Window *Main::window;

	void Main::main()
	{
		glfwWindowHint(GLFW_SAMPLES, 16);
		window = new Window("C++ Voxel", 1920, 1080);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			ERROR("GLAD failed");
		glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glDisable(GL_POLYGON_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(.5, .6, .7, 1);
		//glClearColor(.01, .02, .025, 1);
		window->show();
		window->setVSync(true);
		VertexShader *vertShad = new VertexShader(vShad);
		FragmentShader *fragShad = new FragmentShader(fShad);
		glProg = new Program();
		glProg->attachShader(vertShad);
		glProg->attachShader(fragShad);
		glProg->link();
		glProg->use();
		fogDistanceLocation = glProg->getUniformLocation("fogDistance");
		texCoordsLocation = glProg->getAttribLocation("vertexUV");
		texCoordsLocation->setVertexAttribArray(true);
		vertexesLocation = glProg->getAttribLocation("vertexPosition");
		vertexesLocation->setVertexAttribArray(true);
		colorsLocation = glProg->getAttribLocation("vertexColor");
		colorsLocation->setVertexAttribArray(true);
		mvpLocation = glProg->getUniformLocation("MVP");
		mLocation = glProg->getUniformLocation("M");
		{
			glm::mat4 osef(1);
			mLocation->setMat4f(osef);
		}
		vLocation = glProg->getUniformLocation("V");
		ProgramLocation *texLocation = glProg->getAttribLocation("tex");
		char *datas;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("terrain.png", datas, width, height))
			ERROR("Failed to read terrain.png");
		terrain = new Texture(datas, width, height);
		delete[] (datas);
		terrain->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrain->getTextureID());
		GLint osef = 0;
		texLocation->setVec1i(osef);
		World *world = new World();
		ProgramLocation *timeFactorLocation = glProg->getUniformLocation("timeFactor");
		fogDistanceLocation->setVec1f(16 * 6);
		int64_t lastFrame = System::nanotime();
		while (!window->closeRequested())
		{
			nanotime = System::nanotime();
			frameDelta = nanotime - lastFrame;
			lastFrame = nanotime;
			timeFactorLocation->setVec1f(nanotime / 1000000000.);
			window->clearScreen();
			world->tick();
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

