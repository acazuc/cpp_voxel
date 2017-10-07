#include "GuiTitle.h"
#include "TitleScreen/TitleScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiTitle::GuiTitle()
	: singleplayer(0, 0, "Singleplayer")
	, multiplayer(0, 0, "Multiplayer")
	, texturePack(0, 0, "Mods and Texture Packs")
	, options(0, 0, "Options...")
	, quit(0, 0, "Quit Game")
	{
		this->logo.setTexture(Gui::getLogoTex());
		this->logo.setProgram(Main::getGuiShader().program);
		this->logo.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->logo.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->logo.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->logo.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->logo.setSize(256 * 4, 48 * 4);
		this->logo.setTexSize(1, .1875);
		this->logo.setTexPos(0, 0);
		this->singleplayer.setCallback(&GuiTitle::singleplayerCallback);
		this->multiplayer.setCallback(&GuiTitle::multiplayerCallback);
		this->texturePack.setCallback(&GuiTitle::texturePackCallback);
	}

	void GuiTitle::singleplayerCallback()
	{
		TitleScreen::setMode(TITLE_SCREEN_SINGLEPLAYER);
	}

	void GuiTitle::multiplayerCallback()
	{
		//
	}

	void GuiTitle::texturePackCallback()
	{
		//
	}

	void GuiTitle::draw()
	{
		this->logo.setPos((Main::getWindow()->getWidth() - 256 * 4) / 2, 19 * 8);
		this->logo.draw(Gui::getMat());
		int32_t offset = 48 * 4 + 19 * 8 * 2;
		this->singleplayer.setPos((Main::getWindow()->getWidth() - this->singleplayer.getWidth() * 4) / 2, offset);
		this->singleplayer.draw(Gui::getMat());
		this->multiplayer.setPos((Main::getWindow()->getWidth() - this->multiplayer.getWidth() * 4) / 2, offset + 24 * 4);
		this->multiplayer.draw(Gui::getMat());
		this->texturePack.setPos((Main::getWindow()->getWidth() - this->texturePack.getWidth() * 4) / 2, offset + 48 * 4);
		this->texturePack.draw(Gui::getMat());
	}

	void GuiTitle::mouseMove(bool &alreadyHovered)
	{
		this->singleplayer.mouseMove(alreadyHovered);
		this->multiplayer.mouseMove(alreadyHovered);
		this->texturePack.mouseMove(alreadyHovered);
	}

	bool GuiTitle::mouseDown(MouseEvent &event)
	{
		if (this->singleplayer.mouseDown(event))
			return (true);
		if (this->multiplayer.mouseDown(event))
			return (true);
		if (this->texturePack.mouseDown(event))
			return (true);
		return (false);
	}

}
