#include "Particle.h"
#include "World/ChunkBlock.h"
#include "World/World.h"

namespace voxel
{

	Particle::Particle(World &world, Chunk *chunk, glm::vec3 pos, glm::vec2 size, glm::vec3 vel, glm::vec2 uv, glm::vec2 uvSize, uint8_t light)
	: Entity(world, chunk)
	, uvSize(uvSize)
	, size(size)
	, uv(uv)
	, age(0)
	{
		this->gravity = .04;
		setSize(glm::vec3(.2, .2, .2));
		setPos(pos);
		this->posDst = vel;
		this->duration = 4 / (rand() * .9 / RAND_MAX + .1);
		float lightVal = ChunkBlock::getLightValue(light);
		this->light = glm::vec3(lightVal);
	}

	void Particle::tick()
	{
		if (age > this->duration)
		{
			this->deleted = true;
			return;
		}
		age++;
		Entity::tick();
		this->posDst *= .98;
		if (this->isOnFloor)
			this->posDst *= .7;
	}

	void Particle::draw(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<glm::vec2> &texCoords, std::vector<GLuint> &indices, glm::mat4 &playerMat)
	{
		if (this->deleted)
			return;
		glm::vec3 pos = getRealPos();
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec4(pos, 1) + playerMat * glm::vec4(-this->size.x / 2, -this->size.y / 2, 0, 1));
		vertexes.push_back(glm::vec4(pos, 1) + playerMat * glm::vec4(+this->size.x / 2, -this->size.y / 2, 0, 1));
		vertexes.push_back(glm::vec4(pos, 1) + playerMat * glm::vec4(+this->size.x / 2, +this->size.y / 2, 0, 1));
		vertexes.push_back(glm::vec4(pos, 1) + playerMat * glm::vec4(-this->size.x / 2, +this->size.y / 2, 0, 1));
		colors.push_back(this->light);
		colors.push_back(this->light);
		colors.push_back(this->light);
		colors.push_back(this->light);
		texCoords.push_back(glm::vec2(this->uv.x, this->uv.y + this->uvSize.y));
		texCoords.push_back(glm::vec2(this->uv.x + this->uvSize.x, this->uv.y + this->uvSize.y));
		texCoords.push_back(glm::vec2(this->uv.x + this->uvSize.x, this->uv.y));
		texCoords.push_back(glm::vec2(this->uv.x, this->uv.y));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 3);
	}

	void Particle::updateParentChunk()
	{
		int32_t chunkX = World::getChunkCoord(this->pos.x);
		int32_t chunkZ = World::getChunkCoord(this->pos.z);
		if (!this->chunk || (this->chunk->getX() != chunkX && this->chunk->getZ() != chunkZ))
		{
			Chunk *chunk = NULL;
			if (!(chunk = this->world.getChunk(chunkX, chunkZ)))
			{
				this->deleted = true;
				return;
			}
			if (this->chunk)
				this->chunk->getParticlesManager().removeParticle(this);
			this->chunk = chunk;
			this->chunk->getParticlesManager().addParticle(this);
		}
	}

}
