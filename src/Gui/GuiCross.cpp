#include "GuiCross.h"
#include "Main.h"

namespace voxel
{

	GuiCross::GuiCross()
	{
		this->sprite.setTexture(Main::getGui()->getGui());
		this->sprite.setSize(9 * 4, 9 * 4);
		this->sprite.setProgram(Main::getGuiShader().program);
		this->sprite.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->sprite.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->sprite.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->sprite.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->sprite.setTexPos(.94921, .0117);
		this->sprite.setTexSize(.0351, .0351);
	}

	void GuiCross::draw()
	{
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		this->sprite.setPos((Main::getWindow()->getWidth() - 9 * 4) / 2, (Main::getWindow()->getHeight() - 9 * 4) / 2);
		this->sprite.draw(Main::getGui()->getMat());
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}
