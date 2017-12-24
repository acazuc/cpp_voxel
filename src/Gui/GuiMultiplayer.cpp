#include "GuiMultiplayer.h"
#include "TitleScreen/TitleScreen.h"
#include "Main.h"
#include "Gui.h"

using librender::Vec3;

namespace voxel
{

	GuiMultiplayer::GuiMultiplayer()
	: refresh(0, 0, "Refresh")
	, direct(0, 0, "Direct Connect")
	, remove(0, 0, "Delete")
	, cancel(0, 0, "Cancel")
	, join(0, 0, "Join Server")
	, edit(0, 0, "Edit")
	, add(0, 0, "Add Server")
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
		this->join.setSize(100, 20);
		this->join.setCallback(&GuiMultiplayer::joinCallback, NULL);
		this->direct.setSize(100, 20);
		this->direct.setCallback(&GuiMultiplayer::directCallback, NULL);
		this->add.setSize(100, 20);
		this->add.setCallback(&GuiMultiplayer::addCallback, NULL);
		this->edit.setSize(70, 20);
		this->edit.setCallback(&GuiMultiplayer::editCallback, NULL);
		this->remove.setSize(70, 20);
		this->remove.setCallback(&GuiMultiplayer::removeCallback, NULL);
		this->refresh.setSize(70, 20);
		this->refresh.setCallback(&GuiMultiplayer::refreshCallback, NULL);
		this->cancel.setSize(74, 20);
		this->cancel.setCallback(&GuiMultiplayer::cancelCallback, NULL);
	}

	void GuiMultiplayer::joinCallback(void *data)
	{
		(void)data;
	}

	void GuiMultiplayer::directCallback(void *data)
	{
		(void)data;
	}

	void GuiMultiplayer::addCallback(void *data)
	{
		(void)data;
	}

	void GuiMultiplayer::editCallback(void *data)
	{
		(void)data;
	}

	void GuiMultiplayer::removeCallback(void *data)
	{
		(void)data;
	}

	void GuiMultiplayer::refreshCallback(void *data)
	{
		(void)data;
	}

	void GuiMultiplayer::cancelCallback(void *data)
	{
		(void)data;
		TitleScreen::setMode(TITLE_SCREEN_TITLE);
	}

	void GuiMultiplayer::draw()
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
		this->join.setPos(Main::getWindow()->getWidth() / 2 - 154 * Main::getGuiScale(), top);
		this->join.draw(Gui::getMat());
		this->direct.setPos(Main::getWindow()->getWidth() / 2 - 50 * Main::getGuiScale(), top);
		this->direct.draw(Gui::getMat());
		this->add.setPos(Main::getWindow()->getWidth() / 2 + 54 * Main::getGuiScale(), top);
		this->add.draw(Gui::getMat());
		this->edit.setPos(Main::getWindow()->getWidth() / 2 - 154 * Main::getGuiScale(), bot);
		this->edit.draw(Gui::getMat());
		this->remove.setPos(Main::getWindow()->getWidth() / 2 - 74 * Main::getGuiScale(), bot);
		this->remove.draw(Gui::getMat());
		this->refresh.setPos(Main::getWindow()->getWidth() / 2 + 4 * Main::getGuiScale(), bot);
		this->refresh.draw(Gui::getMat());
		this->cancel.setPos(Main::getWindow()->getWidth() / 2 + 80 * Main::getGuiScale(), bot);
		this->cancel.draw(Gui::getMat());
	}

	void GuiMultiplayer::mouseMove(bool &alreadyHovered)
	{
		this->join.mouseMove(alreadyHovered);
		this->direct.mouseMove(alreadyHovered);
		this->add.mouseMove(alreadyHovered);
		this->edit.mouseMove(alreadyHovered);
		this->remove.mouseMove(alreadyHovered);
		this->refresh.mouseMove(alreadyHovered);
		this->cancel.mouseMove(alreadyHovered);
	}

	bool GuiMultiplayer::mouseDown(MouseEvent &event)
	{
		if (this->join.mouseDown(event))
			return (true);
		if (this->direct.mouseDown(event))
			return (true);
		if (this->add.mouseDown(event))
			return (true);
		if (this->edit.mouseDown(event))
			return (true);
		if (this->remove.mouseDown(event))
			return (true);
		if (this->refresh.mouseDown(event))
			return (true);
		if (this->cancel.mouseDown(event))
			return (true);
		return (false);
	}

	bool GuiMultiplayer::mouseUp(MouseEvent &event)
	{
		(void)event;
		return (false);
	}

	bool GuiMultiplayer::keyDown(KeyEvent &event)
	{
		if (event.key == GLFW_KEY_ESCAPE)
		{
			TitleScreen::setMode(TITLE_SCREEN_TITLE);
			return (true);
		}
		return (false);
	}

}
