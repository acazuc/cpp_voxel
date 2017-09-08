#ifndef BODY_PART_H
# define BODY_PART_H

# include <librender/Shader/VertexBuffer.h>
# include <glm/mat4x4.hpp>
# include <glm/vec3.hpp>
# include <glm/vec2.hpp>

using librender::VertexBuffer;

namespace voxel
{

	class World;

	class BodyPart
	{

	private:
		VertexBuffer texCoordsBuffer;
		VertexBuffer vertexesBuffer;
		VertexBuffer indicesBuffer;
		glm::vec3 pos;
		glm::vec3 rot;

	public:
		BodyPart(glm::vec3 pos, glm::vec3 size, glm::vec2 uv);
		void draw(World *world, glm::mat4 model);
		void setPos(glm::vec3 pos) {this->pos = pos;};
		void setRot(glm::vec3 rot) {this->rot = rot;};
		void setRotX(float rotX) {this->rot.x = rotX;};
		void setRotY(float rotY) {this->rot.y = rotY;};
		void setRotZ(float rotZ) {this->rot.z = rotZ;};

	};

}

#endif
