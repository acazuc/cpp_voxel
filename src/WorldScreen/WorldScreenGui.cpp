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
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		Gui::updateMat();
		glm::vec3 pos(this->worldScreen.getWorld()->getPlayer().getPos());
		pos.y += .72 + 2. / 16;
		ChunkBlock *block = this->worldScreen.getWorld()->getBlockAt(pos);
		if (block && (block->getType() == 8 || block->getType() == 9))
		{
			uint8_t light = this->worldScreen.getWorld()->getLightAt(pos);
			Color color(ChunkBlock::getLightValue(light), .5);
			this->water.setColor(color);
			this->water.setSize(Main::getWindow()->getWidth(), Main::getWindow()->getHeight());
			this->water.draw(Gui::getMat());
		}
		this->hearts.draw();
		this->cross.draw();
		this->bar.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

}
