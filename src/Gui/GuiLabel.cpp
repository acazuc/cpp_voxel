#include "GuiLabel.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiLabel::GuiLabel(int32_t x, int32_t y, std::string text)
	: x(x)
	, y(y)
	{
		this->text.setFont(Main::getFont());
		this->text.setProgram(Main::getGuiShader().program);
		this->text.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->text.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->text.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->text.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->text.setShadowSize(1);
		this->text.setShadowPos(Main::getGuiScale(), Main::getGuiScale());
		this->text.setText(text);
		this->text.setColor(Gui::getTextColor('f'));
		this->text.setShadowColor(Gui::getTextShadowColor('f'));
	}

	void GuiLabel::draw(Mat4 &viewProj)
	{
		this->text.draw(viewProj);
	}

	void GuiLabel::setText(std::string text)
	{
		this->text.setText(text);
	}

	void GuiLabel::setPos(int32_t x, int32_t y)
	{
		this->x = x;
		this->y = y;
		this->text.setPos(this->x, this->y);
	}

	void GuiLabel::setColor(char color)
	{
		this->text.setColor(Gui::getTextColor(color));
		this->text.setShadowColor(Gui::getTextShadowColor(color));
	}

	void GuiLabel::setShadow(bool shadow)
	{
		this->text.setShadowSize(shadow ? 1 : 0);
	}

}
