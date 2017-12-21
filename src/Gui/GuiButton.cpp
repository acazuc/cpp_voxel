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
	, callbackData(NULL)
	, disabled(false)
	, hover(false)
	{
		this->background.setTexture(Gui::getGuiTex());
		this->background.setProgram(Main::getGuiShader().program);
		this->background.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->background.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->background.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->background.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->background.addEntry(&this->bgLeft);
		this->background.addEntry(&this->bgRight);
		this->bgLeft.setTexX(0);
		this->bgLeft.setTexSize(width / 2 / 256., 20 / 256.);
		this->bgRight.setTexX((200 - width / 2) / 256.);
		this->bgRight.setTexSize(width / 2 / 256., 20 / 256.);
		this->bgRight.setX(width / 2 * Main::getGuiScale());
		this->text.setText(text);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiButton::draw(glm::mat4 &viewProj)
	{
		if (this->disabled)
		{
			this->text.setColor('7');
			this->bgLeft.setTexY(46 / 256.);
			this->bgRight.setTexY(46 / 256.);
		}
		else if (this->hover)
		{
			this->text.setColor('e');
			this->bgLeft.setTexY(86 / 256.);
			this->bgRight.setTexY(86 / 256.);
		}
		else
		{
			this->text.setColor('f');
			this->bgLeft.setTexY(66 / 256.);
			this->bgRight.setTexY(66 / 256.);
		}
		this->bgLeft.setSize(this->width / 2 * Main::getGuiScale(), this->height * Main::getGuiScale());
		this->bgRight.setSize(this->width / 2 * Main::getGuiScale(), this->height * Main::getGuiScale());
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
		if (this->callback)
			this->callback(this->callbackData);
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
		this->bgLeft.setTexSize(width / 2 / 256., 20 / 256.);
		this->bgRight.setTexSize(width / 2 / 256., 20 / 256.);
		this->bgRight.setX(this->width / 2 * Main::getGuiScale());
		this->bgRight.setTexX((200 - width / 2) / 256.);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiButton::setDisabled(bool disabled)
	{
		if (this->disabled == disabled)
			return;
		this->disabled = disabled;
	}

}
