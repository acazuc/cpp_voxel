#ifndef CLOUDS_H
# define CLOUDS_H

# include <librender/Shader/VertexBuffer.h>
# include <glm/vec3.hpp>
# include <vector>

using librender::VertexBuffer;

namespace voxel
{

	class World;

	class Clouds
	{

	private:
		World &world;
		std::vector<bool> parts;
		VertexBuffer vertexesBuffer;
		VertexBuffer colorsBuffer;
		uint32_t partsHeight;
		uint32_t partsWidth;
		uint32_t verticesNb;
		float lastXOffset;
		float lastPlayerX;
		float lastPlayerZ;
		bool isPartFilled(int32_t x, int32_t y);
		void drawFaceUp(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y);
		void drawFaceDown(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y);
		void drawFaceLeft(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y);
		void drawFaceRight(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y);
		void drawFaceFront(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y);
		void drawFaceBack(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y);

	public:
		Clouds(World &world);
		~Clouds();
		void tick();
		void draw();
		void rebuild();

	};

}

#endif
