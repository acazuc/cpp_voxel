#include "GuiBar.h"
#include "WorldScreen/WorldScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiBar::GuiBar()
	{
		this->spriteBatch.setTexture(Gui::getGuiTex());
		this->spriteBatch.setProgram(Main::getGuiShader().program);
		this->spriteBatch.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->spriteBatch.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->spriteBatch.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->spriteBatch.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->spriteBatch.addEntry(&this->background);
		this->background.setSize(182 * Main::getGuiScale(), 22 * Main::getGuiScale());
		this->background.setTexPos(0, 0);
		this->background.setTexSize(.7109, .0859);
		this->spriteBatch.addEntry(&this->selected);
		this->selected.setSize(24 * Main::getGuiScale(), 24 * Main::getGuiScale());
		this->selected.setTexPos(0, .0859);
		this->selected.setTexSize(.0937, .0937);
		this->selected.setPos((-1 + 20 * 0) * Main::getGuiScale(), -1 * Main::getGuiScale());
	}

	void GuiBar::draw()
	{
		this->spriteBatch.setPos((Main::getWindow()->getWidth() - 182 * Main::getGuiScale()) / 2, Main::getWindow()->getHeight() - 22 * Main::getGuiScale());
		this->spriteBatch.draw(Gui::getMat());
		this->selected.setPos((-1 + 20 * reinterpret_cast<WorldScreen*>(Main::getScreen())->getWorld()->getPlayer().getInventory().getBarPos()) * Main::getGuiScale(), -1 * Main::getGuiScale());
	}

}
