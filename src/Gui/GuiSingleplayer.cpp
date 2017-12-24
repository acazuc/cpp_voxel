#include "GuiSingleplayer.h"
#include "TitleScreen/TitleScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

using librender::Vec3;

namespace voxel
{

	GuiSingleplayer::GuiSingleplayer()
	: recreate(0, 0, "Re-Create")
	, cancel(0, 0, "Cancel")
	, remove(0, 0, "Delete")
	, create(0, 0, "Create New World")
	, edit(0, 0, "Edit")
	, play(0, 0, "Play Selected World")
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
		this->recreate.setSize(72, 20);
		this->recreate.setCallback(&GuiSingleplayer::recreateCallback, NULL);
		this->cancel.setSize(72, 20);
		this->cancel.setCallback(&GuiSingleplayer::cancelCallback, NULL);
		this->remove.setSize(72, 20);
		this->remove.setCallback(&GuiSingleplayer::removeCallback, NULL);
		this->create.setSize(150, 20);
		this->create.setCallback(&GuiSingleplayer::createCallback, NULL);
		this->edit.setSize(72, 20);
		this->edit.setCallback(&GuiSingleplayer::editCallback, NULL);
		this->play.setSize(150, 20);
		this->play.setCallback(&GuiSingleplayer::playCallback, NULL);
	}

	void GuiSingleplayer::recreateCallback(void *data)
	{
		(void)data;
	}

	void GuiSingleplayer::cancelCallback(void *data)
	{
		(void)data;
		TitleScreen::setMode(TITLE_SCREEN_TITLE);
	}

	void GuiSingleplayer::removeCallback(void *data)
	{
		(void)data;
	}

	void GuiSingleplayer::createCallback(void *data)
	{
		(void)data;
	}

	void GuiSingleplayer::editCallback(void *data)
	{
		(void)data;
	}

	void GuiSingleplayer::playCallback(void *data)
	{
		(void)data;
	}

	void GuiSingleplayer::draw()
	{
		this->bgDarker.setPos(0, 16 * Main::getGuiScale() * 2);
		this->bgDarker.setSize(Main::getWindow()->getWidth(), Main::getWindow()->getHeight() - Main::getGuiScale() * 4 * 24);
		this->bgDarker.setTexSize(Main::getWindow()->getWidth() / (16 * 2. * Main::getGuiScale()), (Main::getWindow()->getHeight() - Main::getGuiScale() * 4 * 24) / (16 * 2.f * Main::getGuiScale()));
		this->bgDarker.draw(Gui::getMat());
		Mat4 mat = Gui::getMat();
		mat = Mat4::translate(mat, Vec3((Main::getWindow()->getWidth() - 270 * Main::getGuiScale()) / 2, 17 * Main::getGuiScale() * 2, 0));
		for (uint32_t i = 0; i < this->entries.size(); ++i)
			this->entries[i]->draw(mat);
		this->topGradient.setPos(0, 16 * Main::getGuiScale() * 2);
		this->topGradient.setSize(Main::getWindow()->getWidth(), Main::getGuiScale() * 4);
		this->topGradient.draw(Gui::getMat());
		this->botGradient.setPos(0, Main::getWindow()->getHeight() - 16 * 2 * Main::getGuiScale() * 2 - Main::getGuiScale() * 4);
		this->botGradient.setSize(Main::getWindow()->getWidth(), 4 * Main::getGuiScale());
		this->botGradient.draw(Gui::getMat());
		float top = Main::getWindow()->getHeight() - 26 * 2 * Main::getGuiScale();
		float bot = Main::getWindow()->getHeight() - 14 * 2 * Main::getGuiScale();
		this->play.setPos(Main::getWindow()->getWidth() / 2 - (4 + this->play.getWidth()) * Main::getGuiScale(), top);
		this->play.draw(Gui::getMat());
		this->edit.setPos(Main::getWindow()->getWidth() / 2 - (4 + this->play.getWidth()) * Main::getGuiScale(), bot);
		this->edit.draw(Gui::getMat());
		this->remove.setPos(Main::getWindow()->getWidth() / 2 - (4 + this->play.getWidth() - this->edit.getWidth() - 6) * Main::getGuiScale(), bot);
		this->remove.draw(Gui::getMat());
		this->create.setPos(Main::getWindow()->getWidth() / 2 + 4 * Main::getGuiScale(), top);
		this->create.draw(Gui::getMat());
		this->recreate.setPos(Main::getWindow()->getWidth() / 2 + 4 * Main::getGuiScale(), bot);
		this->recreate.draw(Gui::getMat());
		this->cancel.setPos(Main::getWindow()->getWidth() / 2 + (4 + this->recreate.getWidth() + 6) * Main::getGuiScale(), bot);
		this->cancel.draw(Gui::getMat());
	}

	void GuiSingleplayer::mouseMove(bool &alreadyHovered)
	{
		for (uint32_t i = 0; i < this->entries.size(); ++i)
			this->entries[i]->mouseMove(alreadyHovered);
		this->create.mouseMove(alreadyHovered);
		this->edit.mouseMove(alreadyHovered);
		this->remove.mouseMove(alreadyHovered);
		this->play.mouseMove(alreadyHovered);
		this->recreate.mouseMove(alreadyHovered);
		this->cancel.mouseMove(alreadyHovered);
	}

	bool GuiSingleplayer::mouseDown(MouseEvent &event)
	{
		for (uint32_t i = 0; i < this->entries.size(); ++i)
		{
			if (this->entries[i]->mouseDown(event))
				return (true);
		}
		if (this->create.mouseDown(event))
			return (true);
		if (this->edit.mouseDown(event))
			return (true);
		if (this->remove.mouseDown(event))
			return (true);
		if (this->play.mouseDown(event))
			return (true);
		if (this->recreate.mouseDown(event))
			return (true);
		if (this->cancel.mouseDown(event))
			return (true);
		return (false);
	}

	bool GuiSingleplayer::mouseUp(MouseEvent &event)
	{
		(void)event;
		return (false);
	}

	bool GuiSingleplayer::keyDown(KeyEvent &event)
	{
		if (event.key == GLFW_KEY_ESCAPE)
		{
			TitleScreen::setMode(TITLE_SCREEN_TITLE);
			return (true);
		}
		return (false);
	}

}
