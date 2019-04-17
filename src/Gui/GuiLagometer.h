#ifndef GUI_LAGOMETER_H
# define GUI_LAGOMETER_H

# include <librender/Shader/Sprite/ShaderSprite.h>
# include <librender/Shader/VertexBuffer.h>
# include <cstdint>

using librender::VertexBuffer;
using librender::ShaderSprite;

namespace voxel
{

	class GuiLagometer
	{

	private:
		static float values[512];
		static uint32_t pos;
		VertexBuffer positionBuffer;
		VertexBuffer colorBuffer;
		ShaderSprite background;

	public:
		GuiLagometer();
		void draw();
		static void addValue(float value);

	};

}

#endif
