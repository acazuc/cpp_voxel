#ifndef SKYBOX_H
# define SKYBOX_H

# include <librender/Shader/DataBuffer.h>
# include <librender/Texture.h>

using librender::DataBuffer;
using librender::Texture;

namespace voxel
{

	class World;

	class Skybox
	{

	private:
		DataBuffer moonTexCoordsBuffer;
		DataBuffer sunTexCoordsBuffer;
		DataBuffer moonVertexesBuffer;
		DataBuffer sunVertexesBuffer;
		DataBuffer moonColorsBuffer;
		DataBuffer sunColorsBuffer;
		Texture *moon;
		Texture *sun;
		World &world;
		void initBuffers();

	public:
		Skybox(World &world);
		~Skybox();
		void draw();

	};

}

#endif
