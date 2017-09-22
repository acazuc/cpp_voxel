#ifndef GUI_LABEL_H
# define GUI_LABEL_H

# include <librender/Shader/Text/ShaderText.h>

using librender::ShaderText;

namespace voxel
{

	class GuiLabel
	{

	private:
		ShaderText text;
		int32_t x;
		int32_t y;

	public:
		GuiLabel(int32_t x, int32_t y, std::string text);
		void draw(glm::mat4 &viewProj);
		void setText(std::string text);
		void setPos(int32_t x, int32_t y);
		inline int32_t getX() {return (this->x);};
		inline int32_t getY() {return (this->y);};
		inline int32_t getWidth() {return (this->text.getWidth());};
		inline int32_t getHeight() {return (this->text.getHeight());};
		inline int32_t getLineHeight() {return (this->text.getLineHeight());};

	};

}

#endif