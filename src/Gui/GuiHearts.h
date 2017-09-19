#ifndef GUI_HEARTS_H
# define GUI_HEARTS_H

# include <librender/Shader/Sprite/ShaderSpriteBatchEntry.h>
# include <librender/Shader/Sprite/ShaderSpriteBatch.h>

using librender::ShaderSpriteBatchEntry;
using librender::ShaderSpriteBatch;

namespace voxel
{

	class GuiHearts
	{

	private:
		ShaderSpriteBatchEntry heartsBackgrounds[10];
		ShaderSpriteBatchEntry hearts[10];
		ShaderSpriteBatch spriteBatch;

	public:
		GuiHearts();
		void draw();

	};

}

#endif
