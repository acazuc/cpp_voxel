#include "WorldScreenGui.h"
#include "WorldScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	WorldScreenGui::WorldScreenGui(WorldScreen &worldScreen)
	: worldScreen(worldScreen)
	{
		this->water.setTexture(Gui::getWaterTex());
		this->water.setProgram(Main::getGuiShader().program);
		this->water.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->water.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->water.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->water.setMvpLocation(Main::getGuiShader().mvpLocation);
		Color color(.825, .5);
		this->water.setColor(color);
	}

	void WorldScreenGui::draw()
	{
		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		Gui::updateMat();
		if (this->worldScreen.getWorld()->getPlayer().isEyeInWater())
		{
			uint8_t light = this->worldScreen.getWorld()->getPlayer().getEyeLight();
			Color color(ChunkBlock::getLightValue(light), .25 - .25 * ((15 - light) / 15.));
			this->water.setColor(color);
			this->water.setSize(Main::getWindow()->getWidth(), Main::getWindow()->getHeight());
			this->water.draw(Gui::getMat());
		}
		this->hearts.draw();
		this->cross.draw();
		this->bar.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}

}
