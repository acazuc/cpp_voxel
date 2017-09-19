#include "Gui.h"
#include "Debug.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <libformat/PNG.h>

using librender::TEXTURE_FILTER_NEAREST;
using librender::TEXTURE_WRAP_REPEAT;

namespace voxel
{

	glm::mat4 Gui::mat;
	Texture *Gui::inventoryTex;
	Texture *Gui::iconsTex;
	Texture *Gui::logoTex;
	Texture *Gui::guiTex;
	Texture *Gui::bgTex;

	void Gui::init()
	{
		char *data;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("data/textures/gui/gui.png", data, width, height))
			ERROR("Failed to read sun.png");
		guiTex = new Texture(data, width, height);
		guiTex->bind();
		guiTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		if (!libformat::PNG::read("data/textures/gui/icons.png", data, width, height))
			ERROR("Failed to read icons.png");
		iconsTex = new Texture(data, width, height);
		iconsTex->bind();
		iconsTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		if (!libformat::PNG::read("data/textures/gui/inventory.png", data, width, height))
			ERROR("Failed to read inventory.png");
		inventoryTex = new Texture(data, width, height);
		inventoryTex->bind();
		inventoryTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		if (!libformat::PNG::read("data/textures/gui/logo.png", data, width, height))
			ERROR("Failed to read logo.png");
		logoTex = new Texture(data, width, height);
		logoTex->bind();
		logoTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		delete[] (data);
		if (!libformat::PNG::read("data/textures/gui/background.png", data, width, height))
			ERROR("Failed to read background.png");
		bgTex = new Texture(data, width, height);
		bgTex->bind();
		bgTex->setFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
		bgTex->setWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
		delete[] (data);
	}

	Gui::Gui()
	{
		//
	}

	Gui::~Gui()
	{
		//
	}

	void Gui::draw()
	{
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		this->inventory.draw();
		this->hearts.draw();
		this->cross.draw();
		this->bar.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

	void Gui::updateMat()
	{
		mat = glm::ortho(0.f, (float)Main::getWindow()->getWidth(), (float)Main::getWindow()->getHeight(), 0.f, -2.f, 2.f);
	}

}
