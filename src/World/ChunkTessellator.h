#ifndef CHUNK_TESSELLATOR_H
# define CHUNK_TESSELLATOR_H

# include <librender/Vec/Vec3.h>
# include <librender/Vec/Vec2.h>
# include <librender/GL.h>
# include <vector>

using librender::Vec3;
using librender::Vec2;

namespace voxel
{

	struct ChunkTessellator
	{
		std::vector<GLuint> indices;
		std::vector<Vec2> texCoords;
		std::vector<Vec3> vertexes;
		std::vector<Vec3> colors;
	};

}

#endif
