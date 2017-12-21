#ifndef GUI_SLIDER_H
# define GUI_SLIDER_H

# include "GuiLabel.h"
# include <librender/Shader/Sprite/ShaderSpriteBatchEntry.h>
# include <librender/Shader/Sprite/ShaderSpriteBatch.h>
# include <librender/Shader/Sprite/ShaderSprite.h>
# include <librender/Window/Events.h>

using librender::ShaderSpriteBatchEntry;
using librender::ShaderSpriteBatch;
using librender::ShaderSprite;
using librender::MouseEvent;

namespace voxel
{

	class GuiSlider
	{

	private:
		ShaderSpriteBatchEntry buttonRight;
		ShaderSpriteBatchEntry buttonLeft;
		ShaderSpriteBatch button;
		ShaderSpriteBatchEntry bgRight;
		ShaderSpriteBatchEntry bgLeft;
		ShaderSpriteBatch background;
		GuiLabel text;
		void(*callback)(float per, void* data);
		int32_t height;
		int32_t width;
		int32_t x;
		int32_t y;
		float per;
		void *callbackData;
		bool disabled;
		bool clicked;
		bool hover;

	public:
		GuiSlider(int32_t x, int32_t y, std::string text, int32_t width = 200, int32_t height = 20);
		void draw(glm::mat4 &viewProj);
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		bool mouseUp(MouseEvent &event);
		void setText(std::string text);
		void setPos(int32_t x, int32_t y);
		void setSize(int32_t width, int32_t height);
		void setDisabled(bool disabled);
		inline void setCallback(void(*callback)(float per, void *data), void *data) {this->callback = callback;this->callbackData = data;};
		inline int32_t getHeight() {return (this->height);};
		inline int32_t getWidth() {return (this->width);};
		inline int32_t getX() {return (this->x);};
		inline int32_t getY() {return (this->y);};

	};

}

#endif
