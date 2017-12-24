#include "Particle.h"
#include "World/ChunkBlock.h"
#include "World/World.h"

namespace voxel
{

	Particle::Particle(World &world, Chunk *chunk, Vec3 pos, Vec2 size, Vec3 vel, Vec2 uv, Vec2 uvSize, uint8_t light)
	: Entity(world, chunk)
	, uvSize(uvSize)
	, size(size)
	, uv(uv)
	, age(0)
	{
		this->gravity = .04;
		setSize(Vec3(.2, .2, .2));
		setPos(pos);
		this->posDst = vel;
		this->duration = 4 / (rand() * .9 / RAND_MAX + .1);
		float lightVal = ChunkBlock::getLightValue(light);
		this->light = Vec3(lightVal);
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

	void Particle::draw(std::vector<Vec3> &vertexes, std::vector<Vec3> &colors, std::vector<Vec2> &texCoords, std::vector<GLuint> &indices, Mat4 &playerMat)
	{
		if (this->deleted)
			return;
		Vec3 pos = getRealPos();
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(pos + (playerMat * Vec4(-this->size.x / 2, -this->size.y / 2, 0, 1)).xyz());
		vertexes.push_back(pos + (playerMat * Vec4(+this->size.x / 2, -this->size.y / 2, 0, 1)).xyz());
		vertexes.push_back(pos + (playerMat * Vec4(+this->size.x / 2, +this->size.y / 2, 0, 1)).xyz());
		vertexes.push_back(pos + (playerMat * Vec4(-this->size.x / 2, +this->size.y / 2, 0, 1)).xyz());
		colors.push_back(this->light);
		colors.push_back(this->light);
		colors.push_back(this->light);
		colors.push_back(this->light);
		texCoords.push_back(Vec2(this->uv.x, this->uv.y + this->uvSize.y));
		texCoords.push_back(Vec2(this->uv.x + this->uvSize.x, this->uv.y + this->uvSize.y));
		texCoords.push_back(Vec2(this->uv.x + this->uvSize.x, this->uv.y));
		texCoords.push_back(Vec2(this->uv.x, this->uv.y));
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
