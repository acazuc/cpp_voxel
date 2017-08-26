#ifndef CLOUDS_H
# define CLOUDS_H

# include <librender/Shader/DataBuffer.h>

using librender::DataBuffer;

namespace voxel
{

	class Clouds
	{

	private:
		DataBuffer vertexesBuffer;
		DataBuffer colorsBuffer;

	public:
		Clouds();
		~Clouds();
		void draw();

	};

}

#endif
