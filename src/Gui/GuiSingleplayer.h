#ifndef GUI_SINGLEPLAYER_H
# define GUI_SINGLEPLAYER_H

# include "GuiSingleplayerEntry.h"
# include "GuiButton.h"
# include <librender/Shader/Sprite/ShaderSprite.h>
# include <vector>

using librender::ShaderSprite;

namespace voxel
{

	class GuiSingleplayer
	{

	private:
		std::vector<GuiSingleplayerEntry*> entries;
		ShaderSprite topGradient;
		ShaderSprite botGradient;
		ShaderSprite background;
		ShaderSprite bgDarker;
		GuiButton recreate;
		GuiButton cancel;
		GuiButton remove;
		GuiButton create;
		GuiButton edit;
		GuiButton play;
		int32_t selected;

	public:
		GuiSingleplayer();
		void draw();
		void mouseMove(bool &alreadyHovered);
		bool mouseDown(MouseEvent &event);
		inline int32_t getSelected() {return (this->selected);};
		inline void setSelected(int32_t selected) {this->selected = selected;};

	};

}

#endif
