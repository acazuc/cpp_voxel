#ifndef PARTICLES_MANAGER_H
# define PARTICLES_MANAGER_H

# include "Particle.h"
# include <librender/Shader/VertexBuffer.h>

using librender::VertexBuffer;

namespace voxel
{

	class Chunk;

	class ParticlesManager
	{

	private:
		std::vector<Particle*> particles;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		std::vector<GLuint> indices;
		VertexBuffer *texCoordsBuffer;
		VertexBuffer *vertexesBuffer;
		VertexBuffer *indicesBuffer;
		VertexBuffer *colorsBuffer;
		Chunk &chunk;
		bool initialized;

	public:
		ParticlesManager(Chunk &chunk);
		~ParticlesManager();
		void tick();
		void draw();
		void addParticle(Particle *particle);

	};

}

#endif
