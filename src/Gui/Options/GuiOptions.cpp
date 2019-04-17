#include "GuiOptions.h"
#include "TitleScreen/TitleScreen.h"
#include "Gui/Gui.h"
#include "Main.h"

namespace voxel
{

	GuiOptions::GuiOptions()
	: resources(0, 0, "Resource Packs...", 150)
	, controls(0, 0, "Controls...", 150)
	, language(0, 0, "Language...", 150)
	, sound(0, 0, "Music & Sounds...", 150)
	, video(0, 0, "Video Settings...", 150)
	, chat(0, 0, "Chat Settings...", 150)
	, done(0, 0, "Done")
	, skin(0, 0, "Skin Customization...", 150)
	, fov(0, 0, "FOV: 30", 150)
	, title(0, 0, "Options")
	{
		this->title.setColor('f');
		this->resources.setCallback(&GuiOptions::resourcesCallback, nullptr);
		this->controls.setCallback(&GuiOptions::controlsCallback, nullptr);
		this->language.setCallback(&GuiOptions::languageCallback, nullptr);
		this->sound.setCallback(&GuiOptions::soundCallback, nullptr);
		this->video.setCallback(&GuiOptions::videoCallback, nullptr);
		this->chat.setCallback(&GuiOptions::chatCallback, nullptr);
		this->done.setCallback(&GuiOptions::doneCallback, nullptr);
		this->skin.setCallback(&GuiOptions::skinCallback, nullptr);
		this->fov.setCallback(&GuiOptions::fovCallback, this);
	}

	void GuiOptions::resourcesCallback(void *data)
	{
		(void)data;
	}

	void GuiOptions::controlsCallback(void *data)
	{
		(void)data;
	}

	void GuiOptions::languageCallback(void *data)
	{
		(void)data;
	}

	void GuiOptions::soundCallback(void *data)
	{
		(void)data;
	}

	void GuiOptions::videoCallback(void *data)
	{
		(void)data;
	}

	void GuiOptions::chatCallback(void *data)
	{
		(void)data;
	}

	void GuiOptions::doneCallback(void *data)
	{
		(void)data;
		TitleScreen::setMode(TITLE_SCREEN_TITLE);
	}

	void GuiOptions::skinCallback(void *data)
	{
		(void)data;
	}

	void GuiOptions::fovCallback(float per, void *data)
	{
		(void)data;
		reinterpret_cast<GuiOptions*>(data)->fov.setText("FOV: " + std::to_string(30 + (int)(per * (110 - 30))));
	}

	void GuiOptions::draw()
	{
		this->title.setPos((Main::getWindow()->getWidth() - this->title.getWidth()) / 2, 15 * Main::getGuiScale());
		this->title.draw(Gui::getMat());
		float left = Main::getWindow()->getWidth() / 2 - (this->fov.getWidth() + 5) * Main::getGuiScale();
		float right = Main::getWindow()->getWidth() / 2 + 5 * Main::getGuiScale();
		this->fov.setPos(left, 48 * Main::getGuiScale());
		this->fov.draw(Gui::getMat());
		this->skin.setPos(left, 96 * Main::getGuiScale());
		this->skin.draw(Gui::getMat());
		this->sound.setPos(right, 96 * Main::getGuiScale());
		this->sound.draw(Gui::getMat());
		this->video.setPos(left, 120 * Main::getGuiScale());
		this->video.draw(Gui::getMat());
		this->controls.setPos(right, 120 * Main::getGuiScale());
		this->controls.draw(Gui::getMat());
		this->language.setPos(left, 144 * Main::getGuiScale());
		this->language.draw(Gui::getMat());
		this->chat.setPos(right, 144 * Main::getGuiScale());
		this->chat.draw(Gui::getMat());
		this->resources.setPos(left, 168 * Main::getGuiScale());
		this->resources.draw(Gui::getMat());
		this->done.setPos((Main::getWindow()->getWidth() - this->done.getWidth() * Main::getGuiScale()) / 2, 200 * Main::getGuiScale());
		this->done.draw(Gui::getMat());
	}

	void GuiOptions::mouseMove(bool &alreadyHovered)
	{
		this->resources.mouseMove(alreadyHovered);
		this->controls.mouseMove(alreadyHovered);
		this->language.mouseMove(alreadyHovered);
		this->sound.mouseMove(alreadyHovered);
		this->video.mouseMove(alreadyHovered);
		this->chat.mouseMove(alreadyHovered);
		this->done.mouseMove(alreadyHovered);
		this->skin.mouseMove(alreadyHovered);
		this->fov.mouseMove(alreadyHovered);
	}

	bool GuiOptions::mouseDown(MouseEvent &event)
	{
		if (this->resources.mouseDown(event))
			return true;
		if (this->controls.mouseDown(event))
			return true;
		if (this->language.mouseDown(event))
			return true;
		if (this->sound.mouseDown(event))
			return true;
		if (this->video.mouseDown(event))
			return true;
		if (this->chat.mouseDown(event))
			return true;
		if (this->done.mouseDown(event))
			return true;
		if (this->skin.mouseDown(event))
			return true;
		if (this->fov.mouseDown(event))
			return true;
		return false;
	}

	bool GuiOptions::mouseUp(MouseEvent &event)
	{
		if (this->fov.mouseUp(event))
			return true;
		return false;
	}

	bool GuiOptions::keyDown(KeyEvent &event)
	{
		if (event.key == GLFW_KEY_ESCAPE)
		{
			TitleScreen::setMode(TITLE_SCREEN_TITLE);
			return true;
		}
		return false;
	}

}
