#include "GuiHearts.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiHearts::GuiHearts()
	{
		this->spriteBatch.setTexture(Gui::getIconsTex());
		this->spriteBatch.setProgram(Main::getGuiShader().program);
		this->spriteBatch.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->spriteBatch.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->spriteBatch.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->spriteBatch.setMvpLocation(Main::getGuiShader().mvpLocation);
		for (uint8_t i = 0; i < 10; ++i)
		{
			this->spriteBatch.addEntry(&this->heartsBackgrounds[i]);
			this->heartsBackgrounds[i].setPos(8 * 4 * i, 0);
			this->heartsBackgrounds[i].setSize(9 * 4, 9 * 4);
			this->heartsBackgrounds[i].setTexSize(.0351, .0351);
			this->heartsBackgrounds[i].setTexPos(.0625, 0);
			this->spriteBatch.addEntry(&this->hearts[i]);
			this->hearts[i].setPos(8 * 4 * i, 0);
			this->hearts[i].setSize(9 * 4, 9 * 4);
			this->hearts[i].setTexSize(.0351, .0351);
			this->hearts[i].setTexPos(.2031, 0);
		}
	}

	void GuiHearts::draw()
	{
		this->spriteBatch.setPos((Main::getWindow()->getWidth() - 182 * 4) / 2, Main::getWindow()->getHeight() - 22 * 4 - 1 * 4 - 9 * 4);
		this->spriteBatch.draw(Gui::getMat());
	}

}
