#include "WorldScreenGui.h"
#include "WorldScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	WorldScreenGui::WorldScreenGui(WorldScreen &worldScreen)
	: worldScreen(worldScreen)
	, fpsLabel(0, 0, "")
	, posLabel(0, 24, "")
	, lastChunkUpdates(INT_MAX)
	, lastFps(INT_MAX)
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
		if (this->lastFps != Main::getFps() || this->lastChunkUpdates != Main::getChunkUpdates())
		{
			this->lastFps = Main::getFps();
			this->lastChunkUpdates = Main::getChunkUpdates();
			this->fpsLabel.setText("cpp_voxel (" + std::to_string(Main::getFps()) + " fps, " + std::to_string(Main::getChunkUpdates()) + " chunk updates)");
		}
		this->fpsLabel.draw(Gui::getMat());
		glm::vec3 pos = this->worldScreen.getWorld()->getPlayer().getPos();
		this->posLabel.setPos(0, 40);
		this->posLabel.setText("x: " + std::to_string(pos.x) + "\ny: " + std::to_string(pos.y) + "\nz: " + std::to_string(pos.z));
		this->posLabel.draw(Gui::getMat());
		this->bar.draw();
		this->lagometer.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}

}
