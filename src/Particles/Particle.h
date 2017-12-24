#ifndef PARTICLE_H
# define PARTICLE_H

# include "Entities/Entity.h"
# include <librender/Mat/Mat4.h>
# include <librender/Vec/Vec3.h>
# include <librender/Vec/Vec2.h>
# include <librender/GL.h>
# include <vector>

using librender::Mat4;
using librender::Vec3;
using librender::Vec2;

namespace voxel
{

	class Particle : public Entity
	{

	private:
		Vec2 uvSize;
		Vec3 light;
		Vec2 size;
		Vec2 uv;
		int64_t duration;
		int64_t age;
		virtual void updateParentChunk();

	public:
		Particle(World &world, Chunk *chunk, Vec3 pos, Vec2 size, Vec3 vel, Vec2 uv, Vec2 uvSize, uint8_t light);
		void tick();
		void draw(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<Vec2> &texCoords, std::vector<GLuint> &indices, Mat4 &playerMat);

	};

}

#endif
