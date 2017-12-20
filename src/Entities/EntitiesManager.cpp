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
#include "Entities/Cow.h"
#include "World/Chunk.h"
#include "Debug.h"
#include "Main.h"
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
		Cow::init();
		Texture **textures[14] = {&character, &pigzombie, &skeleton, &chicken, &creeper, &pigman, &spider, &zombie, &ghast, &sheep, &slime, &squid, &cow, &pig};
		std::string names[14] = {"char", "pigzombie", "skeleton", "chicken", "creeper", "pigman", "spider", "zombie", "ghast", "sheep", "slime", "squid", "cow", "pig"};
		char *datas;
		uint32_t width;
		uint32_t height;
		for (uint8_t  i = 0; i < 14; ++i)
		{
			if (!libformat::PNG::read("data/textures/mob/" + names[i] + ".png", datas, width, height))
				ERROR("Failed to read mob/" << names[i] << ".png");
			*(textures[i]) = new Texture();
			(*(textures[i]))->setData(datas, width, height);
			delete[] (datas);
			(*(textures[i]))->bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	void EntitiesManager::clear()
	{
		Pigzombie::clear();
		Skeleton::clear();
		Creeper::clear();
		Zombie::clear();
		Pigman::clear();
		Human::clear();
		Sheep::clear();
		Slime::clear();
		Pig::clear();
		Cow::clear();
		Texture **textures[14] = {&character, &pigzombie, &skeleton, &chicken, &creeper, &pigman, &spider, &zombie, &ghast, &sheep, &slime, &squid, &cow, &pig};
		for (uint8_t i = 0; i < 14; ++i)
			delete (*(textures[i]));
	}

	EntitiesManager::EntitiesManager(Chunk &chunk)
	: chunk(chunk)
	{
		//Empty
	}

	EntitiesManager::~EntitiesManager()
	{
		for (uint32_t i = 0; i < this->entities.size(); ++i)
			delete (this->entities[i]);
	}

	void EntitiesManager::tick()
	{
		for (uint32_t i = 0; i < this->entities.size(); ++i)
		{
			this->entities[i]->tick();
			if (this->entities[i]->isDeleted())
			{
				delete (this->entities[i]);
				this->entities.erase(this->entities.begin() + i);
				i--;
			}
		}
	}

	void EntitiesManager::draw()
	{
		if (!this->entities.size())
		{
			static int a = 0;
			if (!a && this->chunk.getX() == 16 && this->chunk.getZ() == 0)
			{
				a = 1;
				Creeper *creeper = new Creeper(this->chunk.getWorld(), &this->chunk);
				creeper->setPos(glm::vec3(this->chunk.getX() + .5, 128, this->chunk.getZ() + .5));
				addEntity(creeper);
				Human *human = new Human(this->chunk.getWorld(), &this->chunk);
				human->setPos(glm::vec3(this->chunk.getX() + 5.5, 128, this->chunk.getZ() + .5));
				addEntity(human);
				Pig *pig = new Pig(this->chunk.getWorld(), &this->chunk);
				pig->setPos(glm::vec3(this->chunk.getX() + 10.5, 128, this->chunk.getZ() + .5));
				addEntity(pig);
				Pigman *pigman = new Pigman(this->chunk.getWorld(), &this->chunk);
				pigman->setPos(glm::vec3(this->chunk.getX() + 15.5, 128, this->chunk.getZ() + .5));
				addEntity(pigman);
				Pigzombie *pigzombie = new Pigzombie(this->chunk.getWorld(), &this->chunk);
				pigzombie->setPos(glm::vec3(this->chunk.getX() + .5, 128, this->chunk.getZ() + 5.5));
				addEntity(pigzombie);
				Sheep *sheep = new Sheep(this->chunk.getWorld(), &this->chunk);
				sheep->setPos(glm::vec3(this->chunk.getX() + 5.5, 128, this->chunk.getZ() + 5.5));
				addEntity(sheep);
				Skeleton *skeleton = new Skeleton(this->chunk.getWorld(), &this->chunk);
				skeleton->setPos(glm::vec3(this->chunk.getX() + 10.5, 128, this->chunk.getZ() + 5.5));
				addEntity(skeleton);
				Slime *slime = new Slime(this->chunk.getWorld(), &this->chunk);
				slime->setPos(glm::vec3(this->chunk.getX() + 15.5, 128, this->chunk.getZ() + 5.5));
				addEntity(slime);
				Zombie *zombie = new Zombie(this->chunk.getWorld(), &this->chunk);
				zombie->setPos(glm::vec3(this->chunk.getX() + .5, 128, this->chunk.getZ() + 10.5));
				addEntity(zombie);
				Cow *cow = new Cow(this->chunk.getWorld(), &this->chunk);
				cow->setPos(glm::vec3(this->chunk.getX() + 5.5, 128, this->chunk.getZ() + 10.5));
				addEntity(cow);
			}
			else
				return;
		}
		glDisable(GL_CULL_FACE);
		Main::getEntityShader().program->use();
		for (uint32_t i = 0; i < this->entities.size(); ++i)
			this->entities[i]->draw();
		glEnable(GL_CULL_FACE);
	}

	void EntitiesManager::addEntity(Entity *entity)
	{
		this->entities.push_back(entity);
	}

	void EntitiesManager::removeEntity(Entity *entity)
	{
		for (std::vector<Entity*>::iterator iter = this->entities.begin(); iter != this->entities.end(); ++iter)
		{
			if (*iter != entity)
				continue;
			this->entities.erase(iter);
			return;
		}
	}

}
