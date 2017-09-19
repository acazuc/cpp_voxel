#include "GuiBar.h"
#include "Main.h"

namespace voxel
{

	GuiBar::GuiBar()
	{
		this->sprite.setTexture(Main::getGui()->getGui());
		this->sprite.setSize(182 * 4, 22 * 4);
		this->sprite.setProgram(Main::getGuiShader().program);
		this->sprite.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->sprite.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->sprite.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->sprite.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->sprite.setTexPos(0, 0);
		this->sprite.setTexSize(.7109, .0859);
	}

	void GuiBar::draw()
	{
		this->sprite.setPos((Main::getWindow()->getWidth() - 182 * 4) / 2, Main::getWindow()->getHeight() - 22 * 4);
		this->sprite.draw(Main::getGui()->getMat());
	}

}
