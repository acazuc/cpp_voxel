#ifndef BLOCK_H
# define BLOCK_H

# include <librender/GL.h>
# include <glm/glm.hpp>
# include <cstdint>
# include <vector>

# define BLOCK_FACE_FRONT	0x01
# define BLOCK_FACE_BACK	0x02
# define BLOCK_FACE_LEFT	0x04
# define BLOCK_FACE_RIGHT	0x08
# define BLOCK_FACE_UP		0x10
# define BLOCK_FACE_DOWN	0x20

namespace voxel
{

	class Chunk;

	struct BlockLights
	{
		uint8_t f1p1 : 4;
		uint8_t f1p2 : 4;
		uint8_t f1p3 : 4;
		uint8_t f1p4 : 4;
		uint8_t f2p1 : 4;
		uint8_t f2p2 : 4;
		uint8_t f2p3 : 4;
		uint8_t f2p4 : 4;
		uint8_t f3p1 : 4;
		uint8_t f3p2 : 4;
		uint8_t f3p3 : 4;
		uint8_t f3p4 : 4;
		uint8_t f4p1 : 4;
		uint8_t f4p2 : 4;
		uint8_t f4p3 : 4;
		uint8_t f4p4 : 4;
		uint8_t f5p1 : 4;
		uint8_t f5p2 : 4;
		uint8_t f5p3 : 4;
		uint8_t f5p4 : 4;
		uint8_t f6p1 : 4;
		uint8_t f6p2 : 4;
		uint8_t f6p3 : 4;
		uint8_t f6p4 : 4;
	};

	class Block
	{

	private:
		Chunk *chunk;
		uint8_t visibleFaces;
		uint8_t type;
		int32_t x;
		int32_t y;
		int32_t z;
		bool calcLightsIsTransparentSameChunk(int8_t addX, int8_t addY, int8_t addZ);
		bool calcLightsIsTransparentOtherChunk(Chunk *chunk, int32_t newBlockX, int32_t newBlockY, int32_t newBlockZ);
		bool calcLightsIsTransparent(int8_t x, int8_t y, int8_t z);
		void calcLights(BlockLights &lights);
		void calcVisibleFaces();

	public:
		Block(Chunk *chunk, int32_t x, int32_t y, int32_t z, uint8_t type);
		void fillBuffers(std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices);
		inline bool isTransparent() {return (this->type == 0);};

	};

}

#endif
