#include "GuiTitle.h"
#include "TitleScreen/TitleScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiTitle::GuiTitle()
	: singleplayer(0, 0, "Singleplayer")
	, multiplayer(0, 0, "Multiplayer")
	, texturePack(0, 0, "Texture Packs")
	, options(0, 0, "Options...")
	, quit(0, 0, "Quit Game")
	{
		this->logo.setTexture(Gui::getLogoTex());
		this->logo.setProgram(Gui::getShaderSpriteProgram());
		this->logo.setSize(256 * Main::getGuiScale(), 48 * Main::getGuiScale());
		this->logo.setTexSize(1, .175);
		this->logo.setTexPos(0, 0);
		this->singleplayer.setCallback(&GuiTitle::singleplayerCallback, nullptr);
		this->multiplayer.setCallback(&GuiTitle::multiplayerCallback, nullptr);
		this->texturePack.setCallback(&GuiTitle::texturePackCallback, nullptr);
		this->options.setCallback(&GuiTitle::optionsCallback, nullptr);
		this->quit.setCallback(&GuiTitle::quitCallback, nullptr);
		this->options.setSize(98, 20);
		this->quit.setSize(98, 20);
	}

	void GuiTitle::singleplayerCallback(void *data)
	{
		(void)data;
		TitleScreen::setMode(TITLE_SCREEN_SINGLEPLAYER);
	}

	void GuiTitle::multiplayerCallback(void *data)
	{
		(void)data;
		TitleScreen::setMode(TITLE_SCREEN_MULTIPLAYER);
	}

	void GuiTitle::texturePackCallback(void *data)
	{
		(void)data;
		TitleScreen::setMode(TITLE_SCREEN_TEXTURE_PACK);
	}

	void GuiTitle::optionsCallback(void *data)
	{
		(void)data;
		TitleScreen::setMode(TITLE_SCREEN_OPTIONS);
	}

	void GuiTitle::quitCallback(void *data)
	{
		(void)data;
		exit(EXIT_SUCCESS);
	}

	void GuiTitle::draw()
	{
		this->logo.setPos((Main::getWindow()->getWidth() - 256 * Main::getGuiScale()) / 2, 19 * 8);
		this->logo.draw(Gui::getMat());
		int32_t offset = 48 * Main::getGuiScale() + 19 * 8 * 2;
		float left = (Main::getWindow()->getWidth() - this->singleplayer.getWidth() * Main::getGuiScale()) / 2;
		this->singleplayer.setPos(left, offset);
		this->singleplayer.draw(Gui::getMat());
		this->multiplayer.setPos(left, offset + 24 * Main::getGuiScale());
		this->multiplayer.draw(Gui::getMat());
		this->texturePack.setPos(left, offset + 48 * Main::getGuiScale());
		this->texturePack.draw(Gui::getMat());
		this->options.setPos(left, offset + 84 * Main::getGuiScale());
		this->options.draw(Gui::getMat());
		this->quit.setPos(Main::getWindow()->getWidth() / 2 + 2 * Main::getGuiScale(), offset + 84 * Main::getGuiScale());
		this->quit.draw(Gui::getMat());
	}

	void GuiTitle::mouseMove(bool &alreadyHovered)
	{
		this->singleplayer.mouseMove(alreadyHovered);
		this->multiplayer.mouseMove(alreadyHovered);
		this->texturePack.mouseMove(alreadyHovered);
		this->options.mouseMove(alreadyHovered);
		this->quit.mouseMove(alreadyHovered);
	}

	bool GuiTitle::mouseDown(MouseEvent &event)
	{
		if (this->singleplayer.mouseDown(event))
			return true;
		if (this->multiplayer.mouseDown(event))
			return true;
		if (this->texturePack.mouseDown(event))
			return true;
		if (this->options.mouseDown(event))
			return true;
		if (this->quit.mouseDown(event))
			return true;
		return false;
	}

	bool GuiTitle::mouseUp(MouseEvent &event)
	{
		(void)event;
		return false;
	}

}
