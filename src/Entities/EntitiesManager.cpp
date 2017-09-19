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
			*textures[i] = new Texture(datas, width, height);
			delete[] (datas);
			(*textures[i])->bind();
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	EntitiesManager::EntitiesManager(World &world)
	: world(world)
	{
		Creeper *creeper = new Creeper(this->world);
		creeper->setPos(glm::vec3(0, 128, 0));
		addEntity(creeper);
		Human *human = new Human(this->world);
		human->setPos(glm::vec3(5, 128, 0));
		addEntity(human);
		Pig *pig = new Pig(this->world);
		pig->setPos(glm::vec3(10, 128, 0));
		addEntity(pig);
		Pigman *pigman = new Pigman(this->world);
		pigman->setPos(glm::vec3(15, 128, 0));
		addEntity(pigman);
		Pigzombie *pigzombie = new Pigzombie(this->world);
		pigzombie->setPos(glm::vec3(20, 128, 0));
		addEntity(pigzombie);
		Sheep *sheep = new Sheep(this->world);
		sheep->setPos(glm::vec3(25, 128, 0));
		addEntity(sheep);
		Skeleton *skeleton = new Skeleton(this->world);
		skeleton->setPos(glm::vec3(30, 128, 0));
		addEntity(skeleton);
		Slime *slime = new Slime(this->world);
		slime->setPos(glm::vec3(35, 128, 0));
		addEntity(slime);
		Zombie *zombie = new Zombie(this->world);
		zombie->setPos(glm::vec3(40, 128, 0));
		addEntity(zombie);
		Cow *cow = new Cow(this->world);
		cow->setPos(glm::vec3(-5, 128, 0));
		addEntity(cow);
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
			return;
		Main::getEntityShader().program->use();
		glDisable(GL_CULL_FACE);
		for (uint32_t i = 0; i < this->entities.size(); ++i)
		{
			this->entities[i]->draw();
		}
		glEnable(GL_CULL_FACE);
	}

	void EntitiesManager::addEntity(Entity *entity)
	{
		this->entities.push_back(entity);
	}

}
