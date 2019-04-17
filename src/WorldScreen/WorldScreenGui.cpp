#include "WorldScreenGui.h"
#include "WorldScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	WorldScreenGui::WorldScreenGui(WorldScreen &worldScreen)
	: worldScreen(worldScreen)
	, focusedLabel(0, 100, "")
	, fpsLabel(0, 0, "")
	, posLabel(0, 24, "")
	, lastChunkUpdates(INT_MAX)
	, lastFps(INT_MAX)
	, inventoryEnabled(false)
	{
		this->water.setTexture(Gui::getWaterTex());
		this->water.setProgram(Gui::getShaderSpriteProgram());
		Color color(.825, .5);
		this->water.setColor(color);
	}

	void WorldScreenGui::draw()
	{
		glDisable(GL_MULTISAMPLE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
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
		Vec3 pos = this->worldScreen.getWorld()->getPlayer().getPos();
		this->posLabel.setPos(0, 10 * Main::getGuiScale());
		this->posLabel.setText("x: " + std::to_string(pos.x) + "\ny: " + std::to_string(pos.y) + "\nz: " + std::to_string(pos.z));
		this->posLabel.draw(Gui::getMat());
		this->focusedLabel.setPos(0, 39 * Main::getGuiScale());
		ChunkBlock *block = nullptr;
		if (this->worldScreen.getWorld()->getPlayer().getRaycast().isFound())
		{
			Vec3 pos = this->worldScreen.getWorld()->getPlayer().getRaycast().getPos();
			block = this->worldScreen.getWorld()->getBlock(pos.x, pos.y, pos.z);
		}
		std::string focusedName = block && Blocks::getBlock(block->getType()) ? Blocks::getBlock(block->getType())->getDefaultName() : "None";
		this->focusedLabel.setText("focused: " + focusedName);
		this->focusedLabel.draw(Gui::getMat());
		this->bar.draw();
		this->lagometer.draw();
		if (this->inventoryEnabled)
			this->inventory.draw();
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	void WorldScreenGui::mouseScroll(ScrollEvent &event)
	{
		int8_t &barPos = this->worldScreen.getWorld()->getPlayer().getInventory().getBarPos();
		barPos = (barPos - event.yScroll) % 9;
		if (barPos < 0)
			barPos += 9;
	}

	void WorldScreenGui::keyDown(KeyEvent &event)
	{
		if (event.key == GLFW_KEY_E)
			this->inventoryEnabled = !this->inventoryEnabled;
	}

}
