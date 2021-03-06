#ifndef SLIME_H
# define SLIME_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class Slime : public Entity
	{

	private:
		static BodyPart *body;

	public:
		static void init();
		static void clear();
		Slime(World &world, Chunk *chunk);
		void draw();

	};

}

#endif
