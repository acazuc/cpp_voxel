#ifndef BODY_PART_H
# define BODY_PART_H

# include <librender/Shader/VertexBuffer.h>
# include <librender/Mat/Mat4.h>

using librender::VertexBuffer;
using librender::Mat4;
using librender::Vec3;
using librender::Vec2;

namespace voxel
{

	class World;

	class BodyPart
	{

	private:
		VertexBuffer texCoordsBuffer;
		VertexBuffer vertexesBuffer;
		VertexBuffer indicesBuffer;
		Vec3 pos;
		Vec3 rot;

	public:
		BodyPart(Vec3 pos, Vec3 size, Vec2 uv);
		void draw(World *world, Mat4 model);
		void setPos(Vec3 pos) {this->pos = pos;};
		void setRot(Vec3 rot) {this->rot = rot;};
		void setRotX(float rotX) {this->rot.x = rotX;};
		void setRotY(float rotY) {this->rot.y = rotY;};
		void setRotZ(float rotZ) {this->rot.z = rotZ;};

	};

}

#endif
