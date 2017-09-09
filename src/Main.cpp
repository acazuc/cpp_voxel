#include "Main.h"
#include "EntitiesManager.h"
#include "Utils/readfile.h"
#include "Entities/Pig.h"
#include "Entities/Creeper.h"
#include "Entities/Zombie.h"
#include "Entities/Human.h"
#include "Blocks/Blocks.h"
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

	FocusedShader Main::focusedShader;
	BlocksShader Main::blocksShader;
	CloudsShader Main::cloudsShader;
	SkyboxShader Main::skyboxShader;
	EntityShader Main::entityShader;
	glm::vec4 Main::skyColor;
	Texture *Main::terrain;
	Window *Main::window;
	World *Main::world;
	bool Main::smooth = true;
	bool Main::ssao = true;
	int Main::disableTex = 0;

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
		window->setKeyDownCallback(Main::keyDown);
		window->show();
		window->setVSync(true);
		focusedShader.load();
		skyboxShader.load();
		cloudsShader.load();
		blocksShader.load();
		entityShader.load();
		{
			glm::mat4 osef(1);
			blocksShader.program->use();
			blocksShader.mLocation->setMat4f(osef);
			blocksShader.texLocation->setVec1i(0);
			blocksShader.fogDistanceLocation->setVec1f(16 * 14);
			blocksShader.disableTexLocation->setVec1i(0);
			cloudsShader.program->use();
			cloudsShader.mLocation->setMat4f(osef);
			cloudsShader.fogDistanceLocation->setVec1f(16 * 30);
			skyboxShader.program->use();
			skyboxShader.texLocation->setVec1i(0);
			entityShader.program->use();
			entityShader.fogDistanceLocation->setVec1f(16 * 14);
		}
		char *datas;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("data/textures/terrain.png", datas, width, height))
			ERROR("Failed to read terrain.png");
		terrain = new Texture(datas, width, height);
		delete[] (datas);
		glBindTexture(GL_TEXTURE_2D, terrain->getId());
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glActiveTexture(GL_TEXTURE0);
		EntitiesManager::init();
		Blocks::init();
		Main::world = new World();
		int64_t lastFrame = System::nanotime();
		Pig zombie(*world);
		zombie.setPos(0, 128, 0);
		while (!window->closeRequested())
		{
			nanotime = System::nanotime();
			float bgFactor = std::abs(cos(-nanotime / 1000000000. / 60 / 20 * M_PI + M_PI / 4));
			skyColor = glm::vec4(.662 * bgFactor, .796 * bgFactor, .0125 + .875 * bgFactor, 1);
			glClearColor(skyColor.x, skyColor.y, skyColor.z, skyColor.w);
			frameDelta = nanotime - lastFrame;
			lastFrame = nanotime;
			window->clearScreen();
			world->tick();
			world->draw();
			zombie.tick();
			zombie.draw();
			window->pollEvents();
			window->update();
		}
		delete (world);
		delete (window);
	}

	void Main::keyDown(KeyEvent &event)
	{
		if (event.key == GLFW_KEY_P)
		{
			Main::ssao = !Main::ssao;
			Main::world->update();
		}
		if (event.key == GLFW_KEY_O)
		{
			Main::smooth = !Main::smooth;
			Main::world->update();
		}
		if (event.key == GLFW_KEY_I)
		{
			if (Main::disableTex == 1)
				Main::disableTex = 0;
			else
				Main::disableTex = 1;
			blocksShader.program->use();
			blocksShader.disableTexLocation->setVec1i(Main::disableTex);
		}
	}

	void Main::glErrors(std::string err)
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
	}
}

int main(int ac, char **av)
{
	if (ac < 1)
		ERROR("Error while receiving relative path");
	char sep = '/';
#ifdef PLATFORM_WINDOWS
	sep = '\\';
#endif
	char *lastSlash = std::strrchr(av[0], sep);
	if (lastSlash)
		*lastSlash = '\0';
	if (chdir(av[0]))
		ERROR("Failed to change relative path");
	if (!glfwInit())
		ERROR("Failed to init glfw");
	voxel::Main::main();
	exit(EXIT_SUCCESS);
}
