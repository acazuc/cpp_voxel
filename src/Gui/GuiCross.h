#ifndef GUI_CROSS_H
# define GUI_CROSS_H

# include <librender/Shader/Sprite/ShaderSprite.h>

using librender::ShaderSprite;

namespace voxel
{

	class GuiCross
	{

	private:
		ShaderSprite sprite;

	public:
		GuiCross();
		void draw();

	};

}

#endif
