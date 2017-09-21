#include "GuiButton.h"
#include "Main.h"
#include "Gui.h"

namespace voxel
{

	GuiButton::GuiButton(int32_t x, int32_t y, std::string text, int32_t width, int32_t height)
	: text(0, 0, text)
	, height(height)
	, width(width)
	, x(x)
	, y(y)
	, disabled(false)
	{
		this->background.setTexture(Gui::getGuiTex());
		this->background.setProgram(Main::getGuiShader().program);
		this->background.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->background.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->background.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->background.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->background.setSize(this->width * 4, this->height * 4);
		this->background.setTexPos(0, .1796);
		this->background.setTexSize(.7812, .0781);
		this->text.setText(text);
		this->text.setPos(this->x + (this->width * 4 - this->text.getWidth()) / 2, this->y + (this->height * 4 - this->text.getLineHeight()) / 2);
	}

	void GuiButton::draw(glm::mat4 &viewProj)
	{
		if (!this->disabled)
		{
			this->background.setTexPos(0, .2578);
		}
		this->background.draw(viewProj);
		this->text.draw(viewProj);
	}

	void GuiButton::setPos(int32_t x, int32_t y)
	{
		this->x = x;
		this->y = y;
		this->background.setPos(this->x, this->y);
		this->text.setPos(this->x + (this->width * 4 - this->text.getWidth()) / 2, this->y + (this->height * 4 - this->text.getLineHeight()) / 2);
	}

	void GuiButton::setSize(int32_t width, int32_t height)
	{
		this->width = width;
		this->height = height;
		this->text.setPos(this->x + (this->width * 4 - this->text.getWidth()) / 2, this->y + (this->height * 4 - this->text.getLineHeight()) / 2);
	}

	void GuiButton::setDisabled(bool disabled)
	{
		if (this->disabled == disabled)
			return;
		this->disabled = disabled;
		this->background.setTexPos(0, .1796);
	}

}
