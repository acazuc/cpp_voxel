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
		std::vector<GLuint> indices;
		std::vector<Vec2> texCoords;
		std::vector<Vec3> vertexes;
		std::vector<Vec3> colors;
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
		void removeParticle(Particle *particle);

	};

}

#endif
