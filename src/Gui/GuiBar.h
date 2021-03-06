#ifndef GUI_BAR_H
# define GUI_BAR_H

# include <librender/Shader/Sprite/ShaderSpriteBatchEntry.h>
# include <librender/Shader/Sprite/ShaderSpriteBatch.h>

using librender::ShaderSpriteBatchEntry;
using librender::ShaderSpriteBatch;

namespace voxel
{

	class GuiBar
	{

	private:
		ShaderSpriteBatchEntry background;
		ShaderSpriteBatchEntry selected;
		ShaderSpriteBatch spriteBatch;

	public:
		GuiBar();
		void draw();

	};

}

#endif
