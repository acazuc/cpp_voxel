#ifndef SKYBOX_H
# define SKYBOX_H

# include <librender/Shader/VertexBuffer.h>
# include <librender/Texture.h>

using librender::VertexBuffer;
using librender::Texture;

namespace voxel
{

	class World;

	class Skybox
	{

	private:
		VertexBuffer moonTexCoordsBuffer;
		VertexBuffer sunTexCoordsBuffer;
		VertexBuffer moonVertexesBuffer;
		VertexBuffer sunVertexesBuffer;
		VertexBuffer moonColorsBuffer;
		VertexBuffer sunColorsBuffer;
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
