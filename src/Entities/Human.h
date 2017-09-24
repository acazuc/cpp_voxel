#ifndef HUMAN_H
# define HUMAN_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class World;

	class Human : public Entity
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
		static void clear();
		Human(World &world);
		void draw();

	};

}

#endif
