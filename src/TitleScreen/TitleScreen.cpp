#include "TitleScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

using librender::Color;

namespace voxel
{

	enum TitleScreenMode TitleScreen::mode = TITLE_SCREEN_TITLE;

	TitleScreen::TitleScreen()
	{
		glfwSetInputMode(Main::getWindow()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->background.setTexture(Gui::getBgTex());
		this->background.setProgram(Main::getGuiShader().program);
		this->background.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->background.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->background.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->background.setMvpLocation(Main::getGuiShader().mvpLocation);
		Color color(.25);
		this->background.setColor(color);
	}

	void TitleScreen::draw()
	{
		glDisable(GL_MULTISAMPLE);
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		Gui::updateMat();
		this->background.setSize(Main::getWindow()->getWidth(), Main::getWindow()->getHeight());
		this->background.setTexSize(Main::getWindow()->getWidth() / (16 * 8.f), Main::getWindow()->getHeight() / (16 * 8.f));
		this->background.draw(Gui::getMat());
		if (this->mode == TITLE_SCREEN_TITLE)
			this->title.draw();
		else if (this->mode == TITLE_SCREEN_SINGLEPLAYER)
			this->singleplayer.draw();
		this->lagometer.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
	}

	void TitleScreen::mouseMove()
	{
		bool alreadyHovered = false;
		if (this->mode == TITLE_SCREEN_TITLE)
			this->title.mouseMove(alreadyHovered);
		else if (this->mode == TITLE_SCREEN_SINGLEPLAYER)
			this->singleplayer.mouseMove(alreadyHovered);
	}

	void TitleScreen::mouseDown(MouseEvent &event)
	{
		if (this->mode == TITLE_SCREEN_TITLE)
		{
			if (this->title.mouseDown(event))
				return;
		}
		else if (this->mode == TITLE_SCREEN_SINGLEPLAYER)
		{
			if (this->singleplayer.mouseDown(event))
				return;
		}
	}

}
