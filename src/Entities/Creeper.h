#ifndef CREEPER_H
# define CREEPER_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class Creeper : public Entity
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
		Creeper(World &world, Chunk *chunk);
		void draw();

	};

}

#endif
