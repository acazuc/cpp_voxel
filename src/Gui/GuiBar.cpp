#include "GuiBar.h"
#include "Main.h"

namespace voxel
{

	GuiBar::GuiBar()
	{
		this->spriteBatch.setTexture(Main::getGui()->getGuiTex());
		this->spriteBatch.setProgram(Main::getGuiShader().program);
		this->spriteBatch.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->spriteBatch.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->spriteBatch.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->spriteBatch.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->spriteBatch.addEntry(&this->background);
		this->background.setSize(182 * 4, 22 * 4);
		this->background.setTexPos(0, 0);
		this->background.setTexSize(.7109, .0859);
		this->spriteBatch.addEntry(&this->selected);
		this->selected.setSize(24 * 4, 24 * 4);
		this->selected.setTexPos(0, .0859);
		this->selected.setTexSize(.0937, .0937);
		this->selected.setPos((-1 + 20 * 0) * 4, -1 * 4);
	}

	void GuiBar::draw()
	{
		this->spriteBatch.setPos((Main::getWindow()->getWidth() - 182 * 4) / 2, Main::getWindow()->getHeight() - 22 * 4);
		this->spriteBatch.draw(Main::getGui()->getMat());
	}

}
