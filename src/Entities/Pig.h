#ifndef PIG_H
# define PIG_H

# include "./BodyPart.h"
# include "../Entity.h"

namespace voxel
{

	class World;

	class Pig : public Entity
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
		Pig(World &world);
		void draw();

	};

}

#endif
