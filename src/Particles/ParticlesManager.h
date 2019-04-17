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
		std::vector<Vec3> positions;
		std::vector<Vec3> colors;
		std::vector<Vec2> uvs;
		VertexBuffer *positionBuffer;
		VertexBuffer *indiceBuffer;
		VertexBuffer *colorBuffer;
		VertexBuffer *uvBuffer;
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
