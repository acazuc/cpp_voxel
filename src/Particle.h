#ifndef PARTICLE_H
# define PARTICLE_H

# include "Entity.h"

namespace voxel
{

	class Particle : public Entity
	{

	private:
		int64_t duration;
		int64_t age;

	public:
		Particle(World &world, glm::vec3 pos, glm::vec3 vel);
		void tick();
		void draw();

	};

}

#endif
