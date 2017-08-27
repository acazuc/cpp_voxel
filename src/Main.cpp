#include "Main.h"
#include "Utils/readfile.h"
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

int64_t frameDelta;
int64_t nanotime;

namespace voxel
{

	BlocksShader Main::blocksShader;
	CloudsShader Main::cloudsShader;
	SkyboxShader Main::skyboxShader;
	Texture *Main::terrain;
	Window *Main::window;

	void Main::buildBlocksShader()
	{
		std::string vShad = readfile("data/shaders/blocks.vs");
		LOG("building blocks vertex shader");
		VertexShader *vertShad = new VertexShader(vShad.c_str());
		LOG("building blocks fragment shader");
		std::string fShad = readfile("data/shaders/blocks.fs");
		FragmentShader *fragShad = new FragmentShader(fShad.c_str());
		blocksShader.program = new Program();
		blocksShader.program->attachShader(vertShad);
		blocksShader.program->attachShader(fragShad);
		blocksShader.program->link();
		blocksShader.fogDistanceLocation = blocksShader.program->getUniformLocation("fogDistance");
		blocksShader.timeFactorLocation = blocksShader.program->getUniformLocation("timeFactor");
		blocksShader.texCoordsLocation = blocksShader.program->getAttribLocation("vertexUV");
		blocksShader.texCoordsLocation->setVertexAttribArray(true);
		blocksShader.vertexesLocation = blocksShader.program->getAttribLocation("vertexPosition");
		blocksShader.vertexesLocation->setVertexAttribArray(true);
		blocksShader.fogColorLocation = blocksShader.program->getUniformLocation("fogColor");
		blocksShader.colorsLocation = blocksShader.program->getAttribLocation("vertexColor");
		blocksShader.colorsLocation->setVertexAttribArray(true);
		blocksShader.mvpLocation = blocksShader.program->getUniformLocation("MVP");
		blocksShader.texLocation = blocksShader.program->getAttribLocation("tex");
		blocksShader.mLocation = blocksShader.program->getUniformLocation("M");
		blocksShader.vLocation = blocksShader.program->getUniformLocation("V");
	}

	void Main::buildCloudsShader()
	{
		std::string vShad = readfile("data/shaders/clouds.vs");
		LOG("building clouds vertex shader");
		VertexShader *vertShad = new VertexShader(vShad.c_str());
		std::string fShad = readfile("data/shaders/clouds.fs");
		LOG("building clouds fragment shader");
		FragmentShader *fragShad = new FragmentShader(fShad.c_str());
		cloudsShader.program = new Program();
		cloudsShader.program->attachShader(vertShad);
		cloudsShader.program->attachShader(fragShad);
		cloudsShader.program->link();
		cloudsShader.fogDistanceLocation = cloudsShader.program->getUniformLocation("fogDistance");
		cloudsShader.vertexesLocation = cloudsShader.program->getAttribLocation("vertexPosition");
		cloudsShader.vertexesLocation->setVertexAttribArray(true);
		cloudsShader.fogColorLocation = cloudsShader.program->getUniformLocation("fogColor");
		cloudsShader.colorsLocation = cloudsShader.program->getAttribLocation("vertexColor");
		cloudsShader.colorsLocation->setVertexAttribArray(true);
		cloudsShader.mvpLocation = cloudsShader.program->getUniformLocation("MVP");
		cloudsShader.mLocation = cloudsShader.program->getUniformLocation("M");
		cloudsShader.vLocation = cloudsShader.program->getUniformLocation("V");
	}

	void Main::buildSkyboxShader()
	{
		std::string vShad = readfile("data/shaders/skybox.vs");
		LOG("building skybox vertex shader");
		VertexShader *vertShad = new VertexShader(vShad.c_str());
		LOG("building skybox fragment shader");
		std::string fShad = readfile("data/shaders/skybox.fs");
		FragmentShader *fragShad = new FragmentShader(fShad.c_str());
		skyboxShader.program = new Program();
		skyboxShader.program->attachShader(vertShad);
		skyboxShader.program->attachShader(fragShad);
		skyboxShader.program->link();
		skyboxShader.texCoordsLocation = skyboxShader.program->getAttribLocation("vertexUV");
		skyboxShader.texCoordsLocation->setVertexAttribArray(true);
		skyboxShader.vertexesLocation = skyboxShader.program->getAttribLocation("vertexPosition");
		skyboxShader.vertexesLocation->setVertexAttribArray(true);
		skyboxShader.colorsLocation = skyboxShader.program->getAttribLocation("vertexColor");
		skyboxShader.colorsLocation->setVertexAttribArray(true);
		skyboxShader.mvpLocation = skyboxShader.program->getUniformLocation("MVP");
		skyboxShader.texLocation = skyboxShader.program->getAttribLocation("tex");
	}

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
		glClearColor(.662, .796, 1, 1);
		//glClearColor(0, 0, .0125, 1);
		window->show();
		window->setVSync(true);
		buildBlocksShader();
		buildCloudsShader();
		buildSkyboxShader();
		{
			glm::mat4 osef(1);
			blocksShader.program->use();
			blocksShader.mLocation->setMat4f(osef);
			blocksShader.texLocation->setVec1i(0);
			blocksShader.fogColorLocation->setVec4f(.662, .796, 1, 1);
			blocksShader.fogDistanceLocation->setVec1f(16 * 13);
			cloudsShader.program->use();
			cloudsShader.mLocation->setMat4f(osef);
			cloudsShader.fogColorLocation->setVec4f(.662, .796, 1, 1);
			cloudsShader.fogDistanceLocation->setVec1f(16 * 13);
			skyboxShader.program->use();
			skyboxShader.texLocation->setVec1i(0);
		}
		char *datas;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("data/textures/terrain.png", datas, width, height))
			ERROR("Failed to read terrain.png");
		terrain = new Texture(datas, width, height);
		delete[] (datas);
		terrain->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		glActiveTexture(GL_TEXTURE0);
		World *world = new World();
		int64_t lastFrame = System::nanotime();
		while (!window->closeRequested())
		{
			nanotime = System::nanotime();
			frameDelta = nanotime - lastFrame;
			lastFrame = nanotime;
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

