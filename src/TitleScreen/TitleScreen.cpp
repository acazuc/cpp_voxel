#include "TitleScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

using librender::Color;

namespace voxel
{

	TitleScreen::TitleScreen()
	: singleplayer(0, 0, "Singleplayer")
	, multiplayer(0, 0, "Multiplayer")
	, texturePack(0, 0, "Mods and Texture Packs")
	, options(0, 0, "Options...")
	, quit(0, 0, "Quit Game")
	{
		this->background.setTexture(Gui::getBgTex());
		this->background.setProgram(Main::getGuiShader().program);
		this->background.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->background.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->background.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->background.setMvpLocation(Main::getGuiShader().mvpLocation);
		Color color(.25);
		this->background.setColor(color);
		this->logo.setTexture(Gui::getLogoTex());
		this->logo.setProgram(Main::getGuiShader().program);
		this->logo.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->logo.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->logo.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->logo.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->logo.setSize(256 * 4, 48 * 4);
		this->logo.setTexSize(1, .1875);
		this->logo.setTexPos(0, 0);
	}

	void TitleScreen::draw()
	{
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		Gui::updateMat();
		this->background.setSize(Main::getWindow()->getWidth(), Main::getWindow()->getHeight());
		this->background.setTexSize(Main::getWindow()->getWidth() / (16 * 8.f), Main::getWindow()->getHeight() / (16 * 8.f));
		this->background.draw(Gui::getMat());
		this->logo.setPos((Main::getWindow()->getWidth() - 256 * 4) / 2, 19 * 8);
		this->logo.draw(Gui::getMat());
		int32_t offset = 48 * 4 + 19 * 8 * 2;
		this->singleplayer.setPos((Main::getWindow()->getWidth() - this->singleplayer.getWidth() * 4) / 2, offset);
		this->singleplayer.draw(Gui::getMat());
		this->multiplayer.setPos((Main::getWindow()->getWidth() - this->multiplayer.getWidth() * 4) / 2, offset + 24 * 4);
		this->multiplayer.draw(Gui::getMat());
		this->texturePack.setPos((Main::getWindow()->getWidth() - this->texturePack.getWidth() * 4) / 2, offset + 48 * 4);
		this->texturePack.draw(Gui::getMat());
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

}
