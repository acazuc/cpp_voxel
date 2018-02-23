#include "GuiInventory.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiInventory::GuiInventory()
	: label(0, 0, "Crafting")
	{
		this->background.setTexture(Gui::getInventoryTex());
		this->background.setSize(176 * Main::getGuiScale(), 166 * Main::getGuiScale());
		this->background.setProgram(Gui::getShaderSpriteProgram());
		this->background.setTexPos(0, 0);
		this->background.setTexSize(.6875, .6484);
		this->label.setColor('0');
		this->label.setShadow(false);
	}

	void GuiInventory::draw()
	{
		int32_t x = (Main::getWindow()->getWidth() - 176 * Main::getGuiScale()) / 2;
		int32_t y = (Main::getWindow()->getHeight() - 166 * Main::getGuiScale()) / 2;
		this->background.setPos(x, y);
		this->background.draw(Gui::getMat());
		this->label.setPos(x + 87 * Main::getGuiScale(), y + 15 * Main::getGuiScale());
		this->label.draw(Gui::getMat());
	}

}
