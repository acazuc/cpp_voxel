#ifndef CHUNK_BLOCK_H
# define CHUNK_BLOCK_H

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
# define BLOCK_SIZE 1

namespace voxel
{

	class Chunk;

	struct BlockLightsLevels
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

	class ChunkBlock
	{

	private:
		uint8_t type;
		bool calcLightsLevelsIsTransparent(Chunk *chunk, glm::vec3 &pos, int8_t x, int8_t y, int8_t z);
		uint8_t calcLightLevel(Chunk *chunk, glm::vec3 &pos, int8_t x, int8_t y, int8_t z);
		void calcAmbientOcclusion(glm::vec3 &pos, BlockLightsLevels &lights, uint8_t visibleFaces, bool *blocksTransparent);
		void calcVisibleFaces(Chunk *chunk, glm::vec3 &pos, uint8_t &visibleFaces);
		void smoothLights(float *lights, uint8_t visibleFaces, BlockLightsLevels &lightsLevels, bool *blocksTransparent, int8_t *blockLights);
		void initLightsLevels(BlockLightsLevels &levels, uint8_t visibleFaces, int8_t *blocksLights);

	public:
		ChunkBlock(uint8_t type);
		ChunkBlock();
		void fillBuffers(Chunk *chunk, glm::vec3 &pos, std::vector<glm::vec3> &vertexes, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices);
		inline bool isTransparent() {return (this->type == 0 || this->type == 8);};
		inline void setType(uint8_t type) {this->type = type;};
		inline uint8_t getType() {return (this->type);};

	};

}

#endif
