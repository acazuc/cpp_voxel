#include "GuiSingleplayerEntry.h"
#include "GuiSingleplayer.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

using librender::Color;

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
		this->background.setProgram(Main::getGuiShader().program);
		this->background.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->background.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->background.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->background.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->background.setTexture(Main::getEmpty());
		this->background.setSize(270 * 4 - 4 * 2, 36 * 4 - 4 * 2);
		this->background.setPos(4, 4);
		this->background.setColor(Color::BLACK);
		this->border.setProgram(Main::getGuiShader().program);
		this->border.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->border.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->border.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->border.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->border.setTexture(Main::getEmpty());
		this->border.setSize(270 * 4, 36 * 4);
		Color color(.666);
		this->border.setColor(color);
		this->image.setProgram(Main::getGuiShader().program);
		this->image.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->image.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->image.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->image.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->image.setTexture(Main::getUnknownPack());
		this->dirDate.setColor('7');
		this->other.setColor('7');
	}

	void GuiSingleplayerEntry::draw(glm::mat4 &mat)
	{
		glm::mat4 m2(mat);
		m2 = glm::translate(m2, glm::vec3(0, this->pos * 36 * 4, 0));
		if (this->parent.getSelected() == this->pos)
		{
			this->border.draw(m2);
			this->background.draw(m2);
		}
		this->image.setSize(16 * 4 * 2, 16 * 4 * 2);
		this->image.setPos(2 * 4, 2 * 4);
		this->image.draw(m2);
		this->name.setPos(37 * 4, 2 * 4);
		this->name.draw(m2);
		this->dirDate.setPos(37 * 4, 13 * 4);
		this->dirDate.draw(m2);
		this->other.setPos(37 * 4, (9 + 13) * 4);
		this->other.draw(m2);
	}

	void GuiSingleplayerEntry::mouseMove(bool &alreadyHovered)
	{
		if (Main::getWindow()->getMouseX() >= (Main::getWindow()->getWidth() - 270 * 4) / 2 && Main::getWindow()->getMouseX() <= (Main::getWindow()->getWidth() + 270 * 4) / 2
				&& Main::getWindow()->getMouseY() >= 17 * 4 * 2 + this->pos * 36 * 4 && Main::getWindow()->getMouseY() <= 17 * 4 * 2 + this->pos * 36 * 4 + 36 * 4)
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
			return (true);
		}
		return (false);
	}

}