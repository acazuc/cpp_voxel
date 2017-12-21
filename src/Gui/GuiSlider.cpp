#include "GuiSlider.h"
#include "Main.h"
#include "Gui.h"

namespace voxel
{

	GuiSlider::GuiSlider(int32_t x, int32_t y, std::string text, int32_t width, int32_t height)
	: text(0, 0, text)
	, height(height)
	, width(width)
	, x(x)
	, y(y)
	, per(0)
	, disabled(false)
	, clicked(false)
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
		this->bgLeft.setTexPos(0, 46 / 256.);
		this->bgLeft.setTexSize(this->width / 2 / 256., 20 / 256.);
		this->bgRight.setTexPos((200 - this->width / 2) / 256., 46 / 256.);
		this->bgRight.setTexSize(this->width / 2 / 256., 20 / 256.);
		this->bgRight.setX(this->width / 2 * Main::getGuiScale());
		this->button.setTexture(Gui::getGuiTex());
		this->button.setProgram(Main::getGuiShader().program);
		this->button.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->button.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->button.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->button.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->button.addEntry(&this->buttonRight);
		this->button.addEntry(&this->buttonLeft);
		this->buttonLeft.setTexX(0);
		this->buttonLeft.setTexSize(8 / 2 / 256., 20 / 256.);
		this->buttonRight.setTexX((200 - 8 / 2) / 256.);
		this->buttonRight.setTexSize(8 / 2 / 256., 20 / 256.);
		this->buttonRight.setX(8 / 2 * Main::getGuiScale());
		this->text.setText(text);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiSlider::draw(glm::mat4 &viewProj)
	{
		if (this->disabled)
		{
			this->text.setColor('7');
			this->buttonLeft.setTexY(46 / 256.);
			this->buttonRight.setTexY(46 / 256.);
		}
		else if (this->hover)
		{
			this->text.setColor('e');
		}
		else
		{
			this->text.setColor('f');
			this->buttonLeft.setTexY(66 / 256.);
			this->buttonRight.setTexY(66 / 256.);
		}
		this->bgLeft.setSize(this->width / 2 * Main::getGuiScale(), this->height * Main::getGuiScale());
		this->bgRight.setSize(this->width / 2 * Main::getGuiScale(), this->height * Main::getGuiScale());
		this->background.draw(viewProj);
		this->buttonLeft.setSize(8 / 2 * Main::getGuiScale(), this->height * Main::getGuiScale());
		this->buttonRight.setSize(8 / 2 * Main::getGuiScale(), this->height * Main::getGuiScale());
		this->button.setPos(this->x + per * (this->width - 8) * Main::getGuiScale(), this->y);
		this->button.draw(viewProj);
		this->text.draw(viewProj);
	}

	void GuiSlider::mouseMove(bool &alreadyHovered)
	{
		if (this->clicked)
		{
			this->per = std::min(1.f, std::max(0.f, ((Main::getWindow()->getMouseX() - this->x) / Main::getGuiScale() - 4) / (float)(this->width - 8)));
			if (this->callback)
				this->callback(this->per, this->callbackData);
		}
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

	bool GuiSlider::mouseDown(MouseEvent &event)
	{
		(void)event;
		if (this->hover)
		{
			this->clicked = true;
			this->per = std::min(1.f, std::max(0.f, ((Main::getWindow()->getMouseX() - this->x) / Main::getGuiScale() - 4) / (float)(this->width - 8)));
			if (this->callback)
				this->callback(this->per, this->callbackData);
			return (true);
		}
		this->clicked = false;
		return (false);
	}

	bool GuiSlider::mouseUp(MouseEvent &event)
	{
		(void)event;
		if (this->clicked || this->hover)
		{
			this->clicked = false;
			return (true);
		}
		return (false);
	}

	void GuiSlider::setText(std::string text)
	{
		this->text.setText(text);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiSlider::setPos(int32_t x, int32_t y)
	{
		this->x = x;
		this->y = y;
		this->background.setPos(this->x, this->y);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiSlider::setSize(int32_t width, int32_t height)
	{
		this->width = width;
		this->height = height;
		this->bgLeft.setTexSize(this->width / 2 / 256., 20 / 256.);
		this->bgRight.setTexSize(this->width / 2 / 256., 20 / 256.);
		this->bgRight.setX(this->width / 2 * Main::getGuiScale());
		this->bgRight.setTexX((200 - this->width / 2) / 256.);
		this->text.setPos(this->x + (this->width * Main::getGuiScale() - this->text.getWidth()) / 2, this->y + (this->height * Main::getGuiScale() - this->text.getLineHeight()) / 2);
	}

	void GuiSlider::setDisabled(bool disabled)
	{
		if (this->disabled == disabled)
			return;
		this->disabled = disabled;
	}

}
