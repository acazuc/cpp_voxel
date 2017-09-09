#include "EntitiesManager.h"
#include "Entities/Pigzombie.h"
#include "Entities/Skeleton.h"
#include "Entities/Creeper.h"
#include "Entities/Zombie.h"
#include "Entities/Pigman.h"
#include "Entities/Human.h"
#include "Entities/Sheep.h"
#include "Entities/Slime.h"
#include "Entities/Pig.h"
#include "Debug.h"
#include <libformat/PNG.h>
#include <librender/GL.h>

namespace voxel
{

	Texture *EntitiesManager::character;
	Texture *EntitiesManager::pigzombie;
	Texture *EntitiesManager::skeleton;
	Texture *EntitiesManager::chicken;
	Texture *EntitiesManager::creeper;
	Texture *EntitiesManager::pigman;
	Texture *EntitiesManager::spider;
	Texture *EntitiesManager::zombie;
	Texture *EntitiesManager::ghast;
	Texture *EntitiesManager::sheep;
	Texture *EntitiesManager::slime;
	Texture *EntitiesManager::squid;
	Texture *EntitiesManager::cow;
	Texture *EntitiesManager::pig;

	void EntitiesManager::init()
	{
		Pigzombie::init();
		Skeleton::init();
		Creeper::init();
		Zombie::init();
		Pigman::init();
		Human::init();
		Sheep::init();
		Slime::init();
		Pig::init();
		Texture **textures[14] = {&character, &pigzombie, &skeleton, &chicken, &creeper, &pigman, &spider, &zombie, &ghast, &sheep, &slime, &squid, &cow, &pig};
		std::string names[14] = {"char", "pigzombie", "skeleton", "chicken", "creeper", "pigman", "spider", "zombie", "ghast", "sheep", "slime", "squid", "cow", "pig"};
		char *datas;
		uint32_t width;
		uint32_t height;
		for (uint8_t  i = 0; i < 14; ++i)
		{
			if (!libformat::PNG::read("data/textures/" + names[i] + ".png", datas, width, height))
				ERROR("Failed to read " << names[i] << ".png");
			*textures[i] = new Texture(datas, width, height);
			delete[] (datas);
			(*textures[i])->bind();
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	void EntitiesManager::tick()
	{
		for (uint32_t i = 0; i < this->entities.size(); ++i)
		{
			this->entities[i]->tick();
		}
	}

	void EntitiesManager::draw()
	{
		for (uint32_t i = 0; i < this->entities.size(); ++i)
		{
			this->entities[i]->draw();
		}
	}

	void EntitiesManager::addEntity(Entity *entity)
	{
		this->entities.push_back(entity);
	}

}
