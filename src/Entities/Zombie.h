#ifndef ZOMBIE_H
# define ZOMBIE_H

# include "./BodyPart.h"
# include "../Entity.h"

namespace voxel
{

	class World;

	class Zombie : public Entity
	{

	private:
		static BodyPart *head;
		static BodyPart *body;
		static BodyPart *armL;
		static BodyPart *armR;
		static BodyPart *legL;
		static BodyPart *legR;

	public:
		static void init();
		Zombie(World &world);
		void draw();

	};

}

#endif
