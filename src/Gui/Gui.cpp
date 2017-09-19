#include "Gui.h"
#include "Debug.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <libformat/PNG.h>

namespace voxel
{

	Texture *Gui::inventoryTex;
	Texture *Gui::iconsTex;
	Texture *Gui::guiTex;

	void Gui::init()
	{
		char *data;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("data/textures/gui/gui.png", data, width, height))
			ERROR("Failed to read sun.png");
		guiTex = new Texture(data, width, height);
		guiTex->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		delete[] (data);
		if (!libformat::PNG::read("data/textures/gui/icons.png", data, width, height))
			ERROR("Failed to read sun.png");
		iconsTex = new Texture(data, width, height);
		iconsTex->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		delete[] (data);
		if (!libformat::PNG::read("data/textures/gui/inventory.png", data, width, height))
			ERROR("Failed to read sun.png");
		inventoryTex = new Texture(data, width, height);
		inventoryTex->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
		this->mat = glm::ortho(0.f, (float)Main::getWindow()->getWidth(), (float)Main::getWindow()->getHeight(), 0.f, -2.f, 2.f);
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		this->inventory.draw();
		this->hearts.draw();
		this->cross.draw();
		this->bar.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

}
