#include "Particle.h"

namespace voxel
{

	Particle::Particle(World &world, glm::vec3 pos, glm::vec3 vel)
	: Entity(world)
	{
		setSize(.2, .2, .2);
		setPos(pos);
		this->posDst = vel;
	}

	void Particle::tick()
	{
		//
	}

	void Particle::draw()
	{
		//
	}

}
