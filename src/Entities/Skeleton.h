#ifndef SKELETON_H
# define SKELETON_H

# include "BodyPart.h"
# include "Entity.h"

namespace voxel
{

	class Skeleton : public Entity
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
		Skeleton(World &world, Chunk *chunk);
		void draw();

	};

}

#endif
