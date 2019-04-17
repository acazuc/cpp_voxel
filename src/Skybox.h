#ifndef SKYBOX_H
# define SKYBOX_H

# include <librender/Shader/VertexBuffer.h>
# include <librender/Texture/Texture.h>

using librender::VertexBuffer;
using librender::Texture;

namespace voxel
{

	class World;

	class Skybox
	{

	private:
		VertexBuffer skyboxPositionBuffer;
		VertexBuffer skyboxIndiceBuffer;
		VertexBuffer moonPositionBuffer;
		VertexBuffer sunPositionBuffer;
		VertexBuffer skyboxColorBuffer;
		VertexBuffer moonColorBuffer;
		VertexBuffer sunColorBuffer;
		VertexBuffer moonUVBuffer;
		VertexBuffer sunUVBuffer;
		Texture *moon;
		Texture *sun;
		World &world;
		void initBuffers();
		void updateSkyboxColors();
		uint32_t getSkyboxIndex(uint32_t x, uint32_t y);

	public:
		Skybox(World &world);
		~Skybox();
		void draw();

	};

}

#endif
