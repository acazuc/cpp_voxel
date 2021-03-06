#include "GuiSingleplayerEntry.h"
#include "GuiSingleplayer.h"
#include "Gui/Gui.h"
#include "Main.h"

using librender::Color;
using librender::Vec3;

namespace voxel
{

	GuiSingleplayerEntry::GuiSingleplayerEntry(GuiSingleplayer &parent, int32_t pos)
	: parent(parent)
	, dirDate(0, 0, "New World (9/28/17 10:00 PM)")
	, other(0, 0, "Creative Mode, Cheats, Version: 0.0.0.1")
	, name(0, 0, "New World")
	, pos(pos)
	, hover(false)
	{
		this->background.setTexture(Main::getEmpty());
		this->background.setProgram(Gui::getShaderSpriteProgram());
		this->background.setSize((270 - 2) * Main::getGuiScale(), (36 - 2) * Main::getGuiScale());
		this->background.setPos(Main::getGuiScale(), Main::getGuiScale());
		this->background.setColor(Color::BLACK);
		this->border.setTexture(Main::getEmpty());
		this->border.setProgram(Gui::getShaderSpriteProgram());
		this->border.setSize(270 * Main::getGuiScale(), 36 * Main::getGuiScale());
		Color color(.666);
		this->border.setColor(color);
		this->image.setTexture(Main::getUnknownPack());
		this->image.setProgram(Gui::getShaderSpriteProgram());
		this->dirDate.setColor('7');
		this->other.setColor('7');
		this->dirDate.setShadow(false);
		this->other.setShadow(false);
		this->name.setShadow(false);
	}

	void GuiSingleplayerEntry::draw(Mat4 &mat)
	{
		Mat4 m2(mat);
		m2 = Mat4::translate(m2, Vec3(0, this->pos * 36 * Main::getGuiScale(), 0));
		if (this->parent.getSelected() == this->pos)
		{
			this->border.draw(m2);
			this->background.draw(m2);
		}
		this->image.setSize(16 * Main::getGuiScale() * 2, 16 * Main::getGuiScale() * 2);
		this->image.setPos(2 * Main::getGuiScale(), 2 * Main::getGuiScale());
		this->image.draw(m2);
		this->name.setPos(37 * Main::getGuiScale(), 2 * Main::getGuiScale());
		this->name.draw(m2);
		this->dirDate.setPos(37 * Main::getGuiScale(), 13 * Main::getGuiScale());
		this->dirDate.draw(m2);
		this->other.setPos(37 * Main::getGuiScale(), (9 + 13) * Main::getGuiScale());
		this->other.draw(m2);
	}

	void GuiSingleplayerEntry::mouseMove(bool &alreadyHovered)
	{
		if (Main::getWindow()->getMouseX() >= (Main::getWindow()->getWidth() - 270 * Main::getGuiScale()) / 2
				&& Main::getWindow()->getMouseX() <= (Main::getWindow()->getWidth() + 270 * Main::getGuiScale()) / 2
				&& Main::getWindow()->getMouseY() >= 17 * Main::getGuiScale() * 2 + this->pos * 36 * Main::getGuiScale()
				&& Main::getWindow()->getMouseY() <= 17 * Main::getGuiScale() * 2 + this->pos * 36 * Main::getGuiScale() + 36 * Main::getGuiScale())
		{
			this->hover = true;
			alreadyHovered = true;
			return;
		}
		this->hover = false;
	}

	bool GuiSingleplayerEntry::mouseDown(MouseEvent &event)
	{
		(void)event;
		if (this->hover)
		{
			this->parent.setSelected(this->pos);
			return true;
		}
		return false;
	}

}
