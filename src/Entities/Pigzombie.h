#ifndef PIGZOMBIE_H
# define PIGZOMBIE_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class World;

	class Pigzombie : public Entity
	{

	private:
		static BodyPart *head;
		static BodyPart *headH;
		static BodyPart *body;
		static BodyPart *armL;
		static BodyPart *armR;
		static BodyPart *legL;
		static BodyPart *legR;

	public:
		static void init();
		static void clear();
		Pigzombie(World &world);
		void draw();

	};

}

#endif
