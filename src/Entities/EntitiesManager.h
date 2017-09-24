#ifndef ENTITIES_MANAGER_H
# define ENTITIES_MANAGER_H

# include "Entity.h"
# include <vector>
# include <librender/Texture.h>

using librender::Texture;

namespace voxel
{

	class World;

	class EntitiesManager
	{

	private:
		static Texture *character;
		static Texture *pigzombie;
		static Texture *skeleton;
		static Texture *chicken;
		static Texture *creeper;
		static Texture *pigman;
		static Texture *spider;
		static Texture *zombie;
		static Texture *ghast;
		static Texture *sheep;
		static Texture *slime;
		static Texture *squid;
		static Texture *cow;
		static Texture *pig;
		std::vector<Entity*> entities;
		World &world;

	public:
		static void init();
		static void clear();
		EntitiesManager(World &world);
		~EntitiesManager();
		void tick();
		void draw();
		void addEntity(Entity *entity);
		static inline Texture *getCharacter() {return (character);};
		static inline Texture *getPigzombie() {return (pigzombie);};
		static inline Texture *getSkeleton() {return (skeleton);};
		static inline Texture *getChicken() {return (chicken);};
		static inline Texture *getCreeper() {return (creeper);};
		static inline Texture *getPigman() {return (pigman);};
		static inline Texture *getSpider() {return (spider);};
		static inline Texture *getZombie() {return (zombie);};
		static inline Texture *getGhast() {return (ghast);};
		static inline Texture *getSheep() {return (sheep);};
		static inline Texture *getSlime() {return (slime);};
		static inline Texture *getSquid() {return (squid);};
		static inline Texture *getCow() {return (cow);};
		static inline Texture *getPig() {return (pig);};

	};

}

#endif
