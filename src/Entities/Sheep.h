#ifndef SHEEP_H
# define SHEEP_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class World;

	class Sheep : public Entity
	{

	private:
		static BodyPart *head;
		static BodyPart *body;
		static BodyPart *legFL;
		static BodyPart *legFR;
		static BodyPart *legBL;
		static BodyPart *legBR;

	public:
		static void init();
		Sheep(World &world);
		void draw();

	};

}

#endif
