#ifndef SHEEP_H
# define SHEEP_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

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
		static void clear();
		Sheep(World &world, Chunk *chunk);
		void draw();

	};

}

#endif
