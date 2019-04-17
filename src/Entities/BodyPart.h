#ifndef BODY_PART_H
# define BODY_PART_H

# include <librender/Shader/VertexBuffer.h>
# include <librender/Shader/VertexArray.h>
# include <librender/Mat/Mat4.h>

using librender::VertexBuffer;
using librender::VertexArray;
using librender::Mat4;
using librender::Vec3;
using librender::Vec2;

namespace voxel
{

	class World;

	class BodyPart
	{

	private:
		VertexBuffer positionBuffer;
		VertexBuffer indiceBuffer;
		VertexBuffer uvBuffer;
		VertexArray vertexArray;
		Vec3 pos;
		Vec3 rot;

	public:
		BodyPart(Vec3 org, Vec3 size, Vec2 uv);
		void draw(World *world, Mat4 model);
		void setPos(Vec3 pos);
		void setRot(Vec3 rot);

	};

}

#endif
