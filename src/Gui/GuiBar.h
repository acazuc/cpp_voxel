#ifndef GUI_BAR_H
# define GUI_BAR_H

# include <librender/Shader/Sprite/ShaderSprite.h>

using librender::ShaderSprite;

namespace voxel
{

	class GuiBar
	{

	private:
		ShaderSprite sprite;

	public:
		GuiBar();
		void draw();

	};

}

#endif
