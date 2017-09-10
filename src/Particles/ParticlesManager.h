#ifndef PARTICLES_MANAGER_H
# define PARTICLES_MANAGER_H

# include "Particle.h"
# include <librender/Shader/VertexBuffer.h>

using librender::VertexBuffer;

namespace voxel
{

	class World;

	class ParticlesManager
	{

	private:
		std::vector<Particle*> particles;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		std::vector<GLuint> indices;
		VertexBuffer texCoordsBuffer;
		VertexBuffer vertexesBuffer;
		VertexBuffer indicesBuffer;
		VertexBuffer colorsBuffer;
		World &world;

	public:
		ParticlesManager(World &world);
		~ParticlesManager();
		void tick();
		void draw();
		void addParticle(Particle *particle);

	};

}

#endif
