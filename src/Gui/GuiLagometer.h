#ifndef GUI_LAGOMETER_H
# define GUI_LAGOMETER_H

# include <librender/Shader/VertexBuffer.h>
# include <cstdint>

using librender::VertexBuffer;

namespace voxel
{

	class GuiLagometer
	{

	private://5px per ms
		static float values[512];
		static uint32_t pos;
		VertexBuffer texCoordsBuffer;
		VertexBuffer vertexesBuffer;
		VertexBuffer colorsBuffer;

	public:
		GuiLagometer();
		void draw();
		static void addValue(float value);

	};

}

#endif
