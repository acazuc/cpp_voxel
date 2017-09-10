#include "PlayerRaycast.h"
#include "Blocks/Blocks.h"
#include "World/World.h"
#include "Debug.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

#define OFFSET .002

namespace voxel
{

	PlayerRaycast::PlayerRaycast(Player &player)
	: player(player)
	, destroyed(0)
	, found(false)
	{
		glm::vec3 vertexes[24];
		vertexes[0] = glm::vec3(            -OFFSET, BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[1] = glm::vec3(BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[2] = glm::vec3(            -OFFSET,             -OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[3] = glm::vec3(BLOCK_SIZE + OFFSET,             -OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[4] = glm::vec3(            -OFFSET,             -OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[5] = glm::vec3(            -OFFSET, BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[6] = glm::vec3(BLOCK_SIZE + OFFSET,             -OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[7] = glm::vec3(BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[8] = glm::vec3(            -OFFSET, BLOCK_SIZE + OFFSET,             -OFFSET);
		vertexes[9] = glm::vec3(BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET,             -OFFSET);
		vertexes[10] = glm::vec3(            -OFFSET,             -OFFSET,             -OFFSET);
		vertexes[11] = glm::vec3(BLOCK_SIZE + OFFSET,             -OFFSET,             -OFFSET);
		vertexes[12] = glm::vec3(            -OFFSET,             -OFFSET,             -OFFSET);
		vertexes[13] = glm::vec3(            -OFFSET, BLOCK_SIZE + OFFSET,             -OFFSET);
		vertexes[14] = glm::vec3(BLOCK_SIZE + OFFSET,             -OFFSET,             -OFFSET);
		vertexes[15] = glm::vec3(BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET,             -OFFSET);
		vertexes[16] = glm::vec3(            -OFFSET,             -OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[17] = glm::vec3(            -OFFSET,             -OFFSET,             -OFFSET);
		vertexes[18] = glm::vec3(BLOCK_SIZE + OFFSET,             -OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[19] = glm::vec3(BLOCK_SIZE + OFFSET,             -OFFSET,             -OFFSET);
		vertexes[20] = glm::vec3(            -OFFSET, BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[21] = glm::vec3(            -OFFSET, BLOCK_SIZE + OFFSET,             -OFFSET);
		vertexes[22] = glm::vec3(BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET);
		vertexes[23] = glm::vec3(BLOCK_SIZE + OFFSET, BLOCK_SIZE + OFFSET,             -OFFSET);
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes, sizeof(vertexes), GL_FLOAT, 3, GL_STATIC_DRAW);
	}

	PlayerRaycast::~PlayerRaycast()
	{
		//
	}

	void PlayerRaycast::tick()
	{
		raycast();
	}

	void PlayerRaycast::draw()
	{
		if (!this->found)
			return;
		Main::getFocusedShader().program->use();
		glm::mat4 model(1);
		model = glm::translate(model, this->pos);
		glm::mat4 mvp = this->player.getProjMat() * this->player.getViewMat() * model;
		Main::getFocusedShader().mvpLocation->setMat4f(mvp);
		Main::getFocusedShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		glLineWidth(2);
		glDrawArrays(GL_LINES, 0, 24);
	}

	static float signum(float val)
	{
		if (val < 0)
			return (-1);
		if (val > 0)
			return (1);
		return (0);
	}

	static float intbound(float pos, float dir)
	{
		if (dir < 0 && dir == std::round(dir))
			return (0);
		if (dir > 0)
		{
			if (pos == 0)
				return (1 / dir);
			return ((std::ceil(pos) - pos) / dir);
		}
		return ((pos - std::floor(pos)) / (-dir));
	}

	void PlayerRaycast::raycast()
	{
		this->found = false;
		//glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
		//glBlendEquation(GL_FUNC_ADD);
		glm::vec3 pos(std::floor(this->player.getPos().x), std::floor(this->player.getPos().y + 0.72), std::floor(this->player.getPos().z));
		glm::vec4 dir = glm::vec4(0, 0, -1, 0) * this->player.getViewMat();
		glm::vec3 step(signum(dir.x), signum(dir.y), signum(dir.z));
		glm::vec3 max(intbound(this->player.getPos().x, dir.x), intbound(this->player.getPos().y + 0.72, dir.y), intbound(this->player.getPos().z, dir.z));
		glm::vec3 delta(step.x / dir.x, step.y / dir.y, step.z / dir.z);
		if (dir.x == 0 && dir.y == 0 && dir.z == 0)
			return;
		float radius = 100;
		uint8_t face = 0;
		while (true)
		{
			if (pos.y >= 0 && pos.y < CHUNK_HEIGHT)
			{
				int32_t chunkX = std::floor(pos.x / CHUNK_WIDTH) * CHUNK_WIDTH;
				int32_t chunkZ = std::floor(pos.z / CHUNK_WIDTH) * CHUNK_WIDTH;
				Chunk *chunk = this->player.getWorld().getChunk(chunkX, chunkZ);
				if (chunk)
				{
					glm::vec3 relative(pos.x - chunkX, pos.y, pos.z - chunkZ);
					ChunkBlock *block = chunk->getBlockAt(glm::vec3(relative));
					if (block && block->getType())
					{
						if (Main::getWindow()->isButtonDown(GLFW_MOUSE_BUTTON_LEFT))
						{
							Block *blockModel = Blocks::getBlock(block->getType());
							if (!blockModel)
								return;
							float texX = blockModel->getTexTopX();
							float texY = blockModel->getTexTopY();
							int32_t nb = 3;
							uint8_t light = chunk->getLightAt(relative);
							for (int32_t x = 0; x < nb; ++x)
							{
								for (int32_t y = 0; y < nb; ++y)
								{
									for (int32_t z = 0; z < nb; ++z)
									{
										glm::vec3 pos2(pos.x + (x + .5) / nb, pos.y + (y + .5) / nb, pos.z + (z + .5) / nb);
										pos.x += std::rand() * .1 / RAND_MAX - .05;
										pos.y += std::rand() * .1 / RAND_MAX - .05;
										pos.z += std::rand() * .1 / RAND_MAX - .05;
										glm::vec2 size((std::rand() * .5 / RAND_MAX + .5) * .15, (std::rand() * .5 / RAND_MAX + .5) * .15);
										glm::vec3 dir(std::rand() * .2 / RAND_MAX - .1, std::rand() * .2 / RAND_MAX, std::rand() * .2 / RAND_MAX - .1);
										glm::vec2 uv(texX, texY);
										uv.x += std::rand() * 1. / 16 * 14 / 16 / RAND_MAX;
										uv.y += std::rand() * 1. / 16 * 14 / 16 / RAND_MAX;
										glm::vec2 uvSize(1. / 16 / 8, 1. / 16 / 8);
										Particle *particle = new Particle(this->player.getWorld(), pos2, size, dir, uv, uvSize, light);
										this->player.getWorld().getParticlesManager().addParticle(particle);
									}
								}
							}
							chunk->destroyBlock(relative);
							return;
						}
						else if (Main::getWindow()->isButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
						{
							glm::vec3 newPos(relative);
							if (face == BLOCK_FACE_LEFT)
								newPos.x -= 1;
							else if (face == BLOCK_FACE_RIGHT)
								newPos.x += 1;
							else if (face == BLOCK_FACE_FRONT)
								newPos.z += 1;
							else if (face == BLOCK_FACE_BACK)
								newPos.z -= 1;
							else if (face == BLOCK_FACE_UP)
								newPos.y += 1;
							else if (face == BLOCK_FACE_DOWN)
								newPos.y -= 1;
							Chunk *newChunk = chunk;
							if (newPos.x < 0)
							{
								newChunk = chunk->getChunkXLess();
								newPos.x = CHUNK_WIDTH - 1;
							}
							else if (newPos.x >= CHUNK_WIDTH)
							{
								newChunk = chunk->getChunkXMore();
								newPos.x = 0;
							}
							else if (newPos.y < 0)
							{
								return;
							}
							else if (newPos.y >= CHUNK_HEIGHT)
							{
								return;
							}
							else if (newPos.z < 0)
							{
								newChunk = chunk->getChunkZLess();
								newPos.z = CHUNK_WIDTH - 1;
							}
							else if (newPos.z >= CHUNK_WIDTH)
							{
								newChunk = chunk->getChunkZMore();
								newPos.z = 0;
							}
							ChunkBlock *block = newChunk->getBlockAt(newPos);
							if (block->getType())
								return;
							newChunk->addBlock(newPos, 2);
							return;
						}
						this->found = true;
						this->face = face;
						this->pos = pos;
						return;
					}
				}
			}
			if (max.x < max.y)
			{
				if (max.x < max.z)
				{
					if (max.x > radius)
						break;
					pos.x += step.x;
					max.x += delta.x;
					face = step.x < 0 ? BLOCK_FACE_RIGHT : BLOCK_FACE_LEFT;
				}
				else
				{
					if (max.z > radius)
						break;
					pos.z += step.z;
					max.z += delta.z;
					face = step.z < 0 ? BLOCK_FACE_FRONT : BLOCK_FACE_BACK;
				}
			}
			else
			{
				if (max.y < max.z)
				{
					if (max.y > radius)
						break;
					pos.y += step.y;
					max.y += delta.y;
					face = step.y < 0 ? BLOCK_FACE_UP : BLOCK_FACE_DOWN;
				}
				else
				{
					if (max.z > radius)
						break;
					pos.z += step.z;
					max.z += delta.z;
					face = step.z < 0 ? BLOCK_FACE_FRONT : BLOCK_FACE_BACK;
				}
			}
		}
	}

}
