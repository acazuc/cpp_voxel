#ifndef GUI_BUTTON_H
# define GUI_BUTTON_H

# include "./GuiLabel.h"
# include <librender/Shader/Sprite/ShaderSprite.h>
# include <librender/Shader/Text/ShaderText.h>
# include <librender/Window/Window.h>
# include <string>

using librender::ShaderSprite;
using librender::ShaderText;
using librender::MouseEvent;

namespace voxel
{

	class GuiButton
	{

	private:
		ShaderSprite background;
		GuiLabel text;
		int32_t height;
		int32_t width;
		int32_t x;
		int32_t y;
		bool disabled;
		bool hover;

	public:
		GuiButton(int32_t x, int32_t y, std::string text, int32_t width = 200, int32_t height = 20);
		void draw(glm::mat4 &viewProj);
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		void setPos(int32_t x, int32_t y);
		void setSize(int32_t width, int32_t height);
		void setDisabled(bool disabled);
		inline int32_t getHeight() {return (this->height);};
		inline int32_t getWidth() {return (this->width);};
		inline int32_t getX() {return (this->x);};
		inline int32_t getY() {return (this->y);};

	};

}

#endif
