#include "GuiButton.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiButton::GuiButton(int32_t x, int32_t y, std::string text, int32_t width, int32_t height)
	: text(0, 0, text)
	, callback(NULL)
	, height(height)
	, width(width)
	, x(x)
	, y(y)
	, disabled(false)
	, hover(false)
	{
		this->background.setTexture(Gui::getGuiTex());
		this->background.setProgram(Main::getGuiShader().program);
		this->background.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->background.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->background.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->background.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->background.setTexPos(0, .1796);
		this->background.setTexSize(.7812, .0781);
		this->text.setText(text);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiButton::draw(glm::mat4 &viewProj)
	{
		if (this->disabled)
		{
			this->text.setColor('7');
			this->background.setTexY(.1796);
		}
		else if (this->hover)
		{
			this->text.setColor('e');
			this->background.setTexY(.3359);
		}
		else
		{
			this->text.setColor('f');
			this->background.setTexY(.2578);
		}
		this->background.setSize(this->width * Main::getGuiScale(), this->height * Main::getGuiScale());
		this->background.draw(viewProj);
		this->text.draw(viewProj);
	}

	void GuiButton::mouseMove(bool &alreadyHovered)
	{
		if (alreadyHovered)
		{
			this->hover = false;
			return;
		}
		if (Main::getWindow()->getMouseX() >= this->x && Main::getWindow()->getMouseX() <= this->x + this->width * Main::getGuiScale()
				&& Main::getWindow()->getMouseY() >= this->y && Main::getWindow()->getMouseY() <= this->y + this->height * Main::getGuiScale())
		{
			this->hover = true;
			alreadyHovered = true;
		}
		else
		{
			this->hover = false;
		}
	}

	bool GuiButton::mouseDown(MouseEvent &event)
	{
		if (!this->hover || event.button != GLFW_MOUSE_BUTTON_LEFT)
			return (false);
		if (callback)
			callback();
		return (true);
	}

	void GuiButton::setPos(int32_t x, int32_t y)
	{
		this->x = x;
		this->y = y;
		this->background.setPos(this->x, this->y);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiButton::setSize(int32_t width, int32_t height)
	{
		this->width = width;
		this->height = height;
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiButton::setDisabled(bool disabled)
	{
		if (this->disabled == disabled)
			return;
		this->disabled = disabled;
		this->background.setTexPos(0, .1796);
	}

}
