#include "GuiSingleplayer.h"
#include "Gui/Gui.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

namespace voxel
{

	GuiSingleplayer::GuiSingleplayer()
	: recreate(0, 0, "")
	, cancel(0, 0, "")
	, remove(0, 0, "")
	, create(0, 0, "")
	, edit(0, 0, "")
	, play(0, 0, "")
	, selected(-1)
	{
		this->topGradient.setProgram(Main::getGuiShader().program);
		this->topGradient.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->topGradient.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->topGradient.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->topGradient.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->topGradient.setTexture(Main::getEmpty());
		this->botGradient.setProgram(Main::getGuiShader().program);
		this->botGradient.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->botGradient.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->botGradient.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->botGradient.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->botGradient.setTexture(Main::getEmpty());
		this->bgDarker.setProgram(Main::getGuiShader().program);
		this->bgDarker.setTexCoordsLocation(Main::getGuiShader().texCoordsLocation);
		this->bgDarker.setVertexesLocation(Main::getGuiShader().vertexesLocation);
		this->bgDarker.setColorsLocation(Main::getGuiShader().colorsLocation);
		this->bgDarker.setMvpLocation(Main::getGuiShader().mvpLocation);
		this->bgDarker.setTexture(Gui::getBgTex());
		Color color(0, 1);
		this->topGradient.setTopColor(color);
		this->botGradient.setBotColor(color);
		color.set(0, 0);
		this->topGradient.setBotColor(color);
		this->botGradient.setTopColor(color);
		color.set(.25 * .5);
		this->bgDarker.setColor(color);
		this->entries.push_back(new GuiSingleplayerEntry(*this, 0));
		this->entries.push_back(new GuiSingleplayerEntry(*this, 1));
		this->entries.push_back(new GuiSingleplayerEntry(*this, 2));
	}

	void GuiSingleplayer::draw()
	{
		this->bgDarker.setPos(0, 16 * Main::getGuiScale() * 2);
		this->bgDarker.setSize(Main::getWindow()->getWidth(), Main::getWindow()->getHeight() - Main::getGuiScale() * 4 * 24);
		this->bgDarker.setTexSize(Main::getWindow()->getWidth() / (16 * 2. * Main::getGuiScale()), (Main::getWindow()->getHeight() - Main::getGuiScale() * 4 * 24) / (16 * 2.f * Main::getGuiScale()));
		this->bgDarker.draw(Gui::getMat());
		glm::mat4 mat = Gui::getMat();
		mat = glm::translate(mat, glm::vec3((Main::getWindow()->getWidth() - 270 * Main::getGuiScale()) / 2, 17 * Main::getGuiScale() * 2, 0));
		for (uint32_t i = 0; i < this->entries.size(); ++i)
			this->entries[i]->draw(mat);
		this->topGradient.setPos(0, 16 * Main::getGuiScale() * 2);
		this->topGradient.setSize(Main::getWindow()->getWidth(), Main::getGuiScale() * 4);
		this->topGradient.draw(Gui::getMat());
		this->botGradient.setPos(0, Main::getWindow()->getHeight() - 16 * 2 * Main::getGuiScale() * 2 - Main::getGuiScale() * 4);
		this->botGradient.setSize(Main::getWindow()->getWidth(), 4 * Main::getGuiScale());
		this->botGradient.draw(Gui::getMat());
	}

	void GuiSingleplayer::mouseMove(bool &alreadyHovered)
	{
		for (uint32_t i = 0; i < this->entries.size(); ++i)
			this->entries[i]->mouseMove(alreadyHovered);
	}

	bool GuiSingleplayer::mouseDown(MouseEvent &event)
	{
		for (uint32_t i = 0; i < this->entries.size(); ++i)
		{
			if (this->entries[i]->mouseDown(event))
				return (true);
		}
		return (false);
	}

}
