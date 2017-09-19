#include "Gui.h"
#include "Debug.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <libformat/PNG.h>

namespace voxel
{

	Texture *Gui::gui;

	void Gui::init()
	{
		char *data;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("data/textures/gui/gui.png", data, width, height))
			ERROR("Failed to read sun.png");
		gui = new Texture(data, width, height);
		gui->bind();
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
		this->cross.draw();
		this->bar.draw();
		glEnable(GL_CULL_FACE);
	}

}
