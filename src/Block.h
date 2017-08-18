#ifndef BLOCK_H
# define BLOCK_H

# include <librender/GL.h>
# include <glm/glm.hpp>
# include <cstdint>
# include <vector>

namespace voxel
{

	class Chunk;

	enum BlockFace
	{
		BLOCK_FACE_FRONT,
		BLOCK_FACE_BACK,
		BLOCK_FACE_LEFT,
		BLOCK_FACE_RIGHT,
		BLOCK_FACE_UP,
		BLOCK_FACE_DOWN
	};

	class Block
	{

	private:
		Chunk *chunk;
		uint8_t type;
		int32_t x;
		int32_t y;
		int32_t z;
		bool isVisibleFace(enum BlockFace face);

	public:
		Block(Chunk *chunk, int32_t x, int32_t y, int32_t z, uint8_t type);
		void fillBuffers(std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices);
		inline bool isTransparent() {return (true);};

	};

}

#endif
