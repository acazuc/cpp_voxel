#ifndef CREEPER_H
# define CREEPER_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class World;

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
		Creeper(World &world);
		void draw();

	};

}

#endif
