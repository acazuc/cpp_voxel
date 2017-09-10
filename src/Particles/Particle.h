#ifndef PARTICLE_H
# define PARTICLE_H

# include "Entities/Entity.h"
# include <glm/mat4x4.hpp>
# include <librender/GL.h>
# include <glm/vec2.hpp>
# include <vector>

namespace voxel
{

	class Particle : public Entity
	{

	private:
		glm::vec2 uvSize;
		glm::vec3 light;
		glm::vec2 size;
		glm::vec2 uv;
		int64_t duration;
		int64_t age;

	public:
		Particle(World &world, glm::vec3 pos, glm::vec2 size, glm::vec3 vel, glm::vec2 uv, glm::vec2 uvSize, uint8_t light);
		void tick();
		void draw(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<glm::vec2> &texCoords, std::vector<GLuint> &indices, glm::mat4 &playerMat);

	};

}

#endif
