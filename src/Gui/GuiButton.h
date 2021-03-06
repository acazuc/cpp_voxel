#ifndef GUI_BUTTON_H
# define GUI_BUTTON_H

# include "./GuiLabel.h"
# include <librender/Shader/Sprite/ShaderSpriteBatchEntry.h>
# include <librender/Shader/Sprite/ShaderSpriteBatch.h>
# include <librender/Shader/Text/ShaderText.h>
# include <librender/Window/Window.h>
# include <string>

using librender::ShaderSpriteBatchEntry;
using librender::ShaderSpriteBatch;
using librender::ShaderText;
using librender::MouseEvent;

namespace voxel
{

	class GuiButton
	{

	private:
		ShaderSpriteBatchEntry bgRight;
		ShaderSpriteBatchEntry bgLeft;
		ShaderSpriteBatch background;
		GuiLabel text;
		void(*callback)(void* data);
		int32_t height;
		int32_t width;
		int32_t x;
		int32_t y;
		void *callbackData;
		bool disabled;
		bool hover;

	public:
		GuiButton(int32_t x, int32_t y, std::string text, int32_t width = 200, int32_t height = 20);
		void draw(Mat4 &viewProj);
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		void setText(std::string text);
		void setPos(int32_t x, int32_t y);
		void setSize(int32_t width, int32_t height);
		void setDisabled(bool disabled);
		inline void setCallback(void(*callback)(void *data), void *data) {this->callback = callback;this->callbackData = data;};
		inline int32_t getHeight() {return this->height;};
		inline int32_t getWidth() {return this->width;};
		inline int32_t getX() {return this->x;};
		inline int32_t getY() {return this->y;};

	};

}

#endif
