#ifndef CHUNK_TESSELLATOR_H
# define CHUNK_TESSELLATOR_H

# include <librender/GL.h>
# include <glm/vec3.hpp>
# include <glm/vec2.hpp>
# include <vector>

namespace voxel
{

	struct ChunkTessellator
	{
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		std::vector<GLuint> indices;
	};

}

#endif
