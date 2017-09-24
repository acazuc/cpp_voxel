#ifndef COW_H
# define COW_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class World;

	class Cow : public Entity
	{

	private:
		static BodyPart *head;
		static BodyPart *body;
		static BodyPart *hornL;
		static BodyPart *hornR;
		static BodyPart *legFL;
		static BodyPart *legFR;
		static BodyPart *legBL;
		static BodyPart *legBR;

	public:
		static void init();
		static void clear();
		Cow(World &world);
		void draw();

	};

}

#endif
