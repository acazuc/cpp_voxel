#include "GuiInventory.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiInventory::GuiInventory()
	{
		this->background.setTexture(Gui::getInventoryTex());
		this->background.setSize(176 * Main::getGuiScale(), 166 * Main::getGuiScale());
		this->background.setProgram(Main::getGuiShader().program);
		this->background.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->background.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->background.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->background.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->background.setTexPos(0, 0);
		this->background.setTexSize(.6875, .6484);
	}

	void GuiInventory::draw()
	{
		return;
		this->background.setPos((Main::getWindow()->getWidth() - 176 * Main::getGuiScale()) / 2, (Main::getWindow()->getHeight() - 166 * Main::getGuiScale()) / 2);
		this->background.draw(Gui::getMat());
	}

}
