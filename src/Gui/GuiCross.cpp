#include "GuiCross.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiCross::GuiCross()
	{
		this->sprite.setTexture(Gui::getGuiTex());
		this->sprite.setSize(15 * Main::getGuiScale(), 15 * Main::getGuiScale());
		this->sprite.setProgram(Main::getGuiShader().program);
		this->sprite.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->sprite.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->sprite.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->sprite.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->sprite.setTexPos(.9375, 0);
		this->sprite.setTexSize(0.0586, 0.0586);
	}

	void GuiCross::draw()
	{
		//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		this->sprite.setPos((Main::getWindow()->getWidth() - 15 * Main::getGuiScale()) / 2, (Main::getWindow()->getHeight() - 15 * Main::getGuiScale()) / 2);
		this->sprite.draw(Gui::getMat());
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}
