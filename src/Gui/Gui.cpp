#include "Gui.h"
#include "Debug.h"
#include "Main.h"
#include <libformat/PNG.h>

using librender::TEXTURE_FILTER_NEAREST;
using librender::TEXTURE_WRAP_REPEAT;

namespace voxel
{

	ShaderSpriteProgram Gui::shaderSpriteProgram;
	ShaderTextProgram Gui::shaderTextProgram;
	Texture *Gui::inventoryTex;
	Texture *Gui::iconsTex;
	Texture *Gui::waterTex;
	Texture *Gui::logoTex;
	Texture *Gui::guiTex;
	Texture *Gui::bgTex;
	Mat4 Gui::mat;

	static Color *textColors[16];
	static Color *textShadowColors[16];

	void Gui::init()
	{
		char *data;
		uint32_t width;
		uint32_t height;
		LOG("loading gui/gui.png");
		if (!libformat::PNG::read("data/textures/gui/gui.png", data, width, height))
			ERROR("Failed to read sun.png");
		guiTex = new Texture(data, width, height);
		guiTex->bind();
		guiTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		LOG("loading gui/icons.png");
		if (!libformat::PNG::read("data/textures/gui/icons.png", data, width, height))
			ERROR("Failed to read icons.png");
		iconsTex = new Texture(data, width, height);
		iconsTex->bind();
		iconsTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		LOG("loading gui/inventory.png");
		if (!libformat::PNG::read("data/textures/gui/inventory.png", data, width, height))
			ERROR("Failed to read inventory.png");
		inventoryTex = new Texture(data, width, height);
		inventoryTex->bind();
		inventoryTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		LOG("loading gui/logo.png");
		if (!libformat::PNG::read("data/textures/title/logo.png", data, width, height))
			ERROR("Failed to read logo.png");
		logoTex = new Texture(data, width, height);
		logoTex->bind();
		logoTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		LOG("loading gui/background.png");
		if (!libformat::PNG::read("data/textures/gui/background.png", data, width, height))
			ERROR("Failed to read background.png");
		bgTex = new Texture(data, width, height);
		bgTex->bind();
		bgTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		bgTex->setWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
		delete[] (data);
		LOG("loading misc/water.png");
		if (!libformat::PNG::read("data/textures/misc/water.png", data, width, height))
			ERROR("Failed to read water.png");
		waterTex = new Texture(data, width, height);
		waterTex->bind();
		waterTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		waterTex->setWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
		delete[] (data);
		textShadowColors[0] = new Color(0.00, 0.00, 0.00);
		textShadowColors[1] = new Color(0.00, 0.00, .165);
		textShadowColors[2] = new Color(0.00, .165, 0.00);
		textShadowColors[3] = new Color(0.00, .165, .165);
		textShadowColors[4] = new Color(.165, 0.00, 0.00);
		textShadowColors[5] = new Color(.165, 0.00, .165);
		textShadowColors[6] = new Color(.165, .165, .000);
		textShadowColors[7] = new Color(.165, .165, .165);
		textShadowColors[8] = new Color(.082, .082, .082);
		textShadowColors[9] = new Color(.082, .082, .247);
		textShadowColors[10] = new Color(.082, .274, .082);
		textShadowColors[11] = new Color(.082, .247, .247);
		textShadowColors[12] = new Color(.247, .082, .082);
		textShadowColors[13] = new Color(.247, .082, .247);
		textShadowColors[14] = new Color(.247, .247, .082);
		textShadowColors[15] = new Color(.247, .247, .247);
		textColors[0] = new Color(.000, .000, .000);
		textColors[1] = new Color(.000, .000, .666);
		textColors[2] = new Color(.000, .666, .000);
		textColors[3] = new Color(.000, .666, .666);
		textColors[4] = new Color(.666, .000, .000);
		textColors[5] = new Color(.666, .000, .666);
		textColors[6] = new Color(1.00, .666, .000);
		textColors[7] = new Color(.666, .666, .666);
		textColors[8] = new Color(.333, .333, .333);
		textColors[9] = new Color(.333, .333, 1.00);
		textColors[10] = new Color(.333, 1.00, .333);
		textColors[11] = new Color(.333, 1.00, 1.00);
		textColors[12] = new Color(1.00, .333, .333);
		textColors[13] = new Color(1.00, .333, 1.00);
		textColors[14] = new Color(1.00, 1.00, .333);
		textColors[15] = new Color(1.00, 1.00, 1.00);
		shaderSpriteProgram.program = Main::getGuiShader().program;
		shaderSpriteProgram.vertexesLocation = &Main::getGuiShader().vertexPositionLocation;
		shaderSpriteProgram.colorsLocation = &Main::getGuiShader().vertexColorLocation;
		shaderSpriteProgram.texCoordsLocation = &Main::getGuiShader().vertexUVLocation;
		shaderSpriteProgram.mvpLocation = &Main::getGuiShader().mvpLocation;
		shaderTextProgram.program = Main::getGuiShader().program;
		shaderTextProgram.vertexesLocation = &Main::getGuiShader().vertexPositionLocation;
		shaderTextProgram.colorsLocation = &Main::getGuiShader().vertexColorLocation;
		shaderTextProgram.texCoordsLocation = &Main::getGuiShader().vertexUVLocation;
		shaderTextProgram.mvpLocation = &Main::getGuiShader().mvpLocation;
	}

	void Gui::clear()
	{
		for (uint8_t i = 0; i < 16; ++i)
		{
			delete (textShadowColors[i]);
			delete (textColors[i]);
		}
		delete (inventoryTex);
		delete (iconsTex);
		delete (waterTex);
		delete (logoTex);
		delete (guiTex);
		delete (bgTex);
	}

	Gui::Gui()
	{
	}

	Gui::~Gui()
	{
	}

	void Gui::draw()
	{
	}

	void Gui::updateMat()
	{
		mat = Mat4::ortho(0.f, (float)Main::getWindow()->getWidth(), (float)Main::getWindow()->getHeight(), 0.f, -2.f, 2.f);
	}

	Color &Gui::getTextColor(char i)
	{
		if (i >= 'A' && i <= 'F')
			i -= 'A' - 'a';
		if (i >= '0' && i <= '9')
			return *textColors[i - '0'];
		if (i >= 'a' && i <= 'f')
			return *textColors[10 + i - 'a'];
		return *textColors[15];
	}

	Color &Gui::getTextShadowColor(char i)
	{
		if (i >= 'A' && i <= 'F')
			i -= 'A' - 'a';
		if (i >= '0' && i <= '9')
			return *textShadowColors[i - '0'];
		if (i >= 'a' && i <= 'f')
			return *textShadowColors[10 + i - 'a'];
		return *textShadowColors[15];
	}

}
