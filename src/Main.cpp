#include "Main.h"
#include "Entities/EntitiesManager.h"
#include "WorldScreen/WorldScreen.h"
#include "TitleScreen/TitleScreen.h"
#include "World/Biomes/Biomes.h"
#include "Gui/GuiLagometer.h"
#include "Utils/readfile.h"
#include "Blocks/Blocks.h"
#include "Utils/System.h"
#include "TickManager.h"
#include "World/World.h"
#include "NBT/NBTFile.h"
#include "Gui/Gui.h"
#include "Debug.h"
#include <cstring>
#include <librender/Font/FontModel.h>
#include <libformat/PNG.h>
#include <GL/glu.h>

using librender::FontModel;
using librender::Mat4;

int64_t nanotime;

namespace voxel
{

	ParticlesShader *Main::particlesShader;
	FocusedShader *Main::focusedShader;
	SunMoonShader *Main::sunMoonShader;
	DroppedShader *Main::droppedShader;
	BlocksShader *Main::blocksShader;
	CloudsShader *Main::cloudsShader;
	SkyboxShader *Main::skyboxShader;
	EntityShader *Main::entityShader;
	BreakShader *Main::breakShader;
	GuiShader *Main::guiShader;
	Texture *Main::unknownPack;
	Texture *Main::terrain;
	Texture *Main::empty;
	Window *Main::window;
	Screen *Main::screen;
	Font *Main::font;
	Vec4 Main::skyColor;
	int64_t Main::chunkUpdates = 0;
	uint8_t Main::guiScale = 3;
	int64_t Main::fps = 0;
	bool Main::smooth = true;
	bool Main::ssao = true;
	int Main::disableTex = 0;

	void Main::main()
	{
		//NBTFile file("./level.dat");
		//file.load();
		//file.printDebug();
		//file.save();
		//glfwWindowHint(GLFW_SAMPLES, 32);
		window = new Window("C++ Voxel", 1920, 1080);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			ERROR("GLAD failed");
		glDisable(GL_POLYGON_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		window->show();
		window->setVSync(true);
		particlesShader = new ParticlesShader();
		particlesShader->load();
		focusedShader = new FocusedShader();
		focusedShader->load();
		sunMoonShader = new SunMoonShader();
		sunMoonShader->load();
		droppedShader = new DroppedShader();
		droppedShader->load();
		skyboxShader = new SkyboxShader();
		skyboxShader->load();
		cloudsShader = new CloudsShader();
		cloudsShader->load();
		blocksShader = new BlocksShader();
		blocksShader->load();
		entityShader = new EntityShader();
		entityShader->load();
		breakShader = new BreakShader();
		breakShader->load();
		guiShader = new GuiShader();
		guiShader->load();
		glErrors("shader");
		{
			Mat4 osef(1);
			blocksShader->program->use();
			blocksShader->mLocation->setMat4f(osef);
			blocksShader->texLocation->setVec1i(0);
			blocksShader->fogDistanceLocation->setVec1f(16 * 14);
			blocksShader->disableTexLocation->setVec1i(0);
			cloudsShader->program->use();
			cloudsShader->mLocation->setMat4f(osef);
			cloudsShader->fogDistanceLocation->setVec1f(16 * 14);
			sunMoonShader->program->use();
			sunMoonShader->texLocation->setVec1i(0);
			entityShader->program->use();
			entityShader->fogDistanceLocation->setVec1f(16 * 14);
			particlesShader->program->use();
			particlesShader->fogDistanceLocation->setVec1f(16 * 14);
			breakShader->program->use();
			breakShader->fogDistanceLocation->setVec1f(16 * 14);
			guiShader->program->use();
			guiShader->texLocation->setVec1i(0);
			droppedShader->program->use();
			droppedShader->fogDistanceLocation->setVec1f(16 * 14);
		}
		char *datas;
		uint32_t width;
		uint32_t height;
		LOG("loading terrain.png");
		if (!libformat::PNG::read("data/textures/terrain.png", datas, width, height))
			ERROR("Failed to read terrain.png");
		terrain = new Texture();
		terrain->setData(datas, width, height);
		delete[] (datas);
		terrain->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16); 
		LOG("loading pack.png");
		if (!libformat::PNG::read("data/textures/pack.png", datas, width, height))
			ERROR("Failed to read unknown.png");
		unknownPack = new Texture();
		unknownPack->setData(datas, width, height);
		delete[] (datas);
		unknownPack->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		datas = new char[4];
		std::memset(datas, 0xff, 4);
		empty = new Texture();
		empty->setData(datas, 1, 1);
		delete[] (datas);
		glActiveTexture(GL_TEXTURE0);
		FontModel *fontModel = new FontModel("./data/minecraftia.ttf");
		font = fontModel->derive(8 * guiScale);
		EntitiesManager::init();
		Blocks::init();
		Biomes::init();
		Gui::init();
		screen = new WorldScreen(new World());
		//screen = new TitleScreen();
		nanotime = System::nanotime();
		int64_t fpsCount = 0;
		int64_t lastFps = nanotime / 1000000000 * 1000000000;
		TickManager::init();
		window->setScrollCallback(&Main::mouseScroll);
		window->setMouseMoveCallback(&Main::mouseMove);
		window->setMouseDownCallback(&Main::mouseDown);
		window->setMouseUpCallback(&Main::mouseUp);
		window->setKeyDownCallback(Main::keyDown);
		while (!window->closeRequested())
		{
			++fpsCount;
			nanotime = System::nanotime();
			if (nanotime - lastFps >= 1000000000)
			{
				lastFps += 1000000000;
				fps = fpsCount;
				LOG("FPS: " << fpsCount);
				fpsCount = 0;
			}
			skyColor = Vec4(.71, .82, 1, 1);
			glClearColor(skyColor.x, skyColor.y, skyColor.z, skyColor.w);
			window->clearScreen();
			TickManager::update();
			for (uint32_t i = 0; i < TickManager::getTicksToDo(); ++i)
				screen->tick();
			screen->draw();
			glErrors("check");
			window->pollEvents();
			int64_t ended = System::nanotime();
			GuiLagometer::addValue((ended - nanotime) / 1000000.);
			window->update();
		}
		EntitiesManager::clear();
		Blocks::clear();
		Biomes::clear();
		Gui::clear();
		delete (particlesShader);
		delete (focusedShader);
		delete (sunMoonShader);
		delete (skyboxShader);
		delete (cloudsShader);
		delete (blocksShader);
		delete (entityShader);
		delete (breakShader);
		delete (guiShader);
		delete (fontModel);
		delete (terrain);
		delete (screen);
		delete (font);
		delete (window);
	}

	void Main::mouseScroll(ScrollEvent &event)
	{
		screen->mouseScroll(event);
	}

	void Main::mouseMove()
	{
		screen->mouseMove();
	}

	void Main::mouseDown(MouseEvent &event)
	{
		screen->mouseDown(event);
	}

	void Main::mouseUp(MouseEvent &event)
	{
		screen->mouseUp(event);
	}

	void Main::keyDown(KeyEvent &event)
	{
		if (event.key == GLFW_KEY_P)
		{
			Main::ssao = !Main::ssao;
			//Main::world->update();
			return;
		}
		if (event.key == GLFW_KEY_O)
		{
			Main::smooth = !Main::smooth;
			//Main::world->update();
			return;
		}
		if (event.key == GLFW_KEY_I)
		{
			if (Main::disableTex == 1)
				Main::disableTex = 0;
			else
				Main::disableTex = 1;
			blocksShader->program->use();
			blocksShader->disableTexLocation->setVec1i(Main::disableTex);
			return;
		}
		screen->keyDown(event);
	}

	void Main::glErrors(std::string err)
	{
		GLenum glErr;
		while ((glErr = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err << ": ";
			if (glErr == GL_INVALID_ENUM)
				std::cerr << "GL_INVALID_ENUM" << std::endl;
			else if (glErr == GL_INVALID_VALUE)
				std::cerr << "GL_INVALID_VALUE" << std::endl;
			else if (glErr == GL_INVALID_OPERATION)
				std::cerr << "GL_INVALID_OPERATION" << std::endl;
			else if (glErr == GL_STACK_OVERFLOW)
				std::cerr << "GL_STACK_OVERFLOW" << std::endl;
			else if (glErr == GL_STACK_UNDERFLOW)
				std::cerr << "GL_STACK_UNDERFLOW" << std::endl;
			else if (glErr == GL_OUT_OF_MEMORY)
				std::cerr << "GL_OUT_OF_MEMORY" << std::endl;
		}
	}
}

int main(int ac, char **av)
{
	if (ac >= 1)
	{
		char sep = '/';
#ifdef PLATFORM_WINDOWS
		sep = '\\';
#endif
		char *lastSlash = std::strrchr(av[0], sep);
		if (lastSlash)
			*lastSlash = '\0';
		if (chdir(av[0]))
			LOG("Failed to change relative path");
	}
	if (!glfwInit())
		ERROR("Failed to init glfw");
	voxel::Main::main();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
