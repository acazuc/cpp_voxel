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
