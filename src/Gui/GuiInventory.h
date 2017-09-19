#ifndef GUI_INVENTORY_H
# define GUI_INVENTORY_H

# include <librender/Shader/Sprite/ShaderSprite.h>

using librender::ShaderSprite;

namespace voxel
{

	class GuiInventory
	{

	private:
		ShaderSprite background;

	public:
		GuiInventory();
		void draw();

	};

}

#endif
