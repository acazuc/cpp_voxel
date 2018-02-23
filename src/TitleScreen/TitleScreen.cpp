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
		this->background.setProgram(Gui::getShaderSpriteProgram());
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
		this->background.setTexSize(Main::getWindow()->getWidth() / (16 * 2. * Main::getGuiScale()), Main::getWindow()->getHeight() / (16 * 2.f * Main::getGuiScale()));
		this->background.draw(Gui::getMat());
		switch (this->mode)
		{
			case TITLE_SCREEN_TITLE:
				this->title.draw();
				break;
			case TITLE_SCREEN_SINGLEPLAYER:
				this->singleplayer.draw();
				break;
			case TITLE_SCREEN_MULTIPLAYER:
				this->multiplayer.draw();
				break;
			case TITLE_SCREEN_OPTIONS:
				this->options.draw();
				break;
			case TITLE_SCREEN_TEXTURE_PACK:
				break;
		}
		this->lagometer.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
	}

	void TitleScreen::mouseMove()
	{
		bool alreadyHovered = false;
		switch (this->mode)
		{
			case TITLE_SCREEN_TITLE:
				this->title.mouseMove(alreadyHovered);
				break;
			case TITLE_SCREEN_SINGLEPLAYER:
				this->singleplayer.mouseMove(alreadyHovered);
				break;
			case TITLE_SCREEN_MULTIPLAYER:
				this->multiplayer.mouseMove(alreadyHovered);
				break;
			case TITLE_SCREEN_OPTIONS:
				this->options.mouseMove(alreadyHovered);
				break;
			case TITLE_SCREEN_TEXTURE_PACK:
				break;
		}
	}

	void TitleScreen::mouseDown(MouseEvent &event)
	{
		switch (this->mode)
		{
			case TITLE_SCREEN_TITLE:
				if (this->title.mouseDown(event))
					return;
				break;
			case TITLE_SCREEN_SINGLEPLAYER:
				if (this->singleplayer.mouseDown(event))
					return;
				break;
			case TITLE_SCREEN_MULTIPLAYER:
				if (this->multiplayer.mouseDown(event))
					return;
				break;
			case TITLE_SCREEN_OPTIONS:
				if (this->options.mouseDown(event))
					return;
				break;
			case TITLE_SCREEN_TEXTURE_PACK:
				break;
		}
	}

	void TitleScreen::mouseUp(MouseEvent &event)
	{
		switch (this->mode)
		{
			case TITLE_SCREEN_TITLE:
				if (this->title.mouseUp(event))
					return;
				break;
			case TITLE_SCREEN_SINGLEPLAYER:
				if (this->singleplayer.mouseUp(event))
					return;
				break;
			case TITLE_SCREEN_MULTIPLAYER:
				if (this->multiplayer.mouseUp(event))
					return;
				break;
			case TITLE_SCREEN_OPTIONS:
				if (this->options.mouseUp(event))
					return;
				break;
			case TITLE_SCREEN_TEXTURE_PACK:
				break;
		}
	}

	void TitleScreen::keyDown(KeyEvent &event)
	{
		switch (this->mode)
		{
			case TITLE_SCREEN_TITLE:
				break;
			case TITLE_SCREEN_SINGLEPLAYER:
				if (this->singleplayer.keyDown(event))
					return;
				break;
			case TITLE_SCREEN_MULTIPLAYER:
				if (this->multiplayer.keyDown(event))
					return;
				break;
			case TITLE_SCREEN_OPTIONS:
				if (this->options.keyDown(event))
					return;
				break;
			case TITLE_SCREEN_TEXTURE_PACK:
				break;
		}
	}

}
