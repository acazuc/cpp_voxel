#ifndef CLOUDS_H
# define CLOUDS_H

# include <librender/Shader/VertexBuffer.h>
# include <librender/Vec/Vec3.h>
# include <vector>

using librender::VertexBuffer;
using librender::Vec3;

namespace voxel
{

	class World;

	class Clouds
	{

	private:
		World &world;
		std::vector<bool> parts;
		VertexBuffer vertexesBuffer;
		VertexBuffer indicesBuffer;
		VertexBuffer colorsBuffer;
		uint32_t partsHeight;
		uint32_t partsWidth;
		uint32_t verticesNb;
		float lastXOffset;
		float lastPlayerX;
		float lastPlayerZ;
		bool isPartFilled(int32_t x, int32_t y);
		void drawFaceUp(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y);
		void drawFaceDown(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y);
		void drawFaceLeft(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y);
		void drawFaceRight(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y);
		void drawFaceFront(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y);
		void drawFaceBack(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y);

	public:
		Clouds(World &world);
		~Clouds();
		void tick();
		void draw();
		void rebuild();

	};

}

#endif
