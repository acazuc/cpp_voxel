#include "PlayerRaycast.h"
#include "Debug.h"
#include "World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

#define WIDTH .501

namespace voxel
{

	PlayerRaycast::PlayerRaycast(Player &player)
	: player(player)
	, found(false)
	{
		glm::vec4 colors[24];
		for (uint32_t i = 0; i < 24; ++i)
			colors[i] = glm::vec4(0, 0, 0, .5);
		this->colorsBuffer.setData(GL_ARRAY_BUFFER, colors, sizeof(colors), GL_FLOAT, 4, GL_STATIC_DRAW);
		glm::vec3 vertexes[24];
		vertexes[0] = glm::vec3(-WIDTH, +WIDTH, +WIDTH);
		vertexes[1] = glm::vec3(+WIDTH, +WIDTH, +WIDTH);
		vertexes[2] = glm::vec3(-WIDTH, -WIDTH, +WIDTH);
		vertexes[3] = glm::vec3(+WIDTH, -WIDTH, +WIDTH);
		vertexes[4] = glm::vec3(-WIDTH, -WIDTH, +WIDTH);
		vertexes[5] = glm::vec3(-WIDTH, +WIDTH, +WIDTH);
		vertexes[6] = glm::vec3(+WIDTH, -WIDTH, +WIDTH);
		vertexes[7] = glm::vec3(+WIDTH, +WIDTH, +WIDTH);
		vertexes[8] = glm::vec3(-WIDTH, +WIDTH, -WIDTH);
		vertexes[9] = glm::vec3(+WIDTH, +WIDTH, -WIDTH);
		vertexes[10] = glm::vec3(-WIDTH, -WIDTH, -WIDTH);
		vertexes[11] = glm::vec3(+WIDTH, -WIDTH, -WIDTH);
		vertexes[12] = glm::vec3(-WIDTH, -WIDTH, -WIDTH);
		vertexes[13] = glm::vec3(-WIDTH, +WIDTH, -WIDTH);
		vertexes[14] = glm::vec3(+WIDTH, -WIDTH, -WIDTH);
		vertexes[15] = glm::vec3(+WIDTH, +WIDTH, -WIDTH);
		vertexes[16] = glm::vec3(-WIDTH, -WIDTH, +WIDTH);
		vertexes[17] = glm::vec3(-WIDTH, -WIDTH, -WIDTH);
		vertexes[18] = glm::vec3(+WIDTH, -WIDTH, +WIDTH);
		vertexes[19] = glm::vec3(+WIDTH, -WIDTH, -WIDTH);
		vertexes[20] = glm::vec3(-WIDTH, +WIDTH, +WIDTH);
		vertexes[21] = glm::vec3(-WIDTH, +WIDTH, -WIDTH);
		vertexes[22] = glm::vec3(+WIDTH, +WIDTH, +WIDTH);
		vertexes[23] = glm::vec3(+WIDTH, +WIDTH, -WIDTH);
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
		model = glm::translate(model, glm::vec3(this->x, this->y, this->z));
		glm::mat4 mvp = this->player.getProjMat() * this->player.getViewMat() * model;
		Main::getFocusedShader().mvpLocation->setMat4f(mvp);
		Main::getFocusedShader().mLocation->setMat4f(model);
		Main::getFocusedShader().vLocation->setMat4f(this->player.getViewMat());
		Main::getFocusedShader().fogColorLocation->setVec4f(Main::getSkyColor());
		Main::getFocusedShader().vertexesLocation->setDataBuffer(this->vertexesBuffer);
		Main::getFocusedShader().colorsLocation->setDataBuffer(this->colorsBuffer);
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
		if (dir > 0)
			return ((round(pos) - pos) / dir);
		return ((pos - round(pos)) / (-dir));
	}

	void PlayerRaycast::raycast()
	{
		this->found = false;
		//glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
		//glBlendEquation(GL_FUNC_ADD);
		glm::vec3 pos(round(this->player.getPosX()), round(this->player.getPosY()), round(this->player.getPosZ()));
		glm::vec4 dir = glm::vec4(0, 0, -1, 0) * this->player.getViewMat();
		glm::vec3 step(signum(dir.x), signum(dir.y), signum(dir.z));
		glm::vec3 max(intbound(this->player.getPosX(), dir.x), intbound(this->player.getPosY(), dir.y), intbound(this->player.getPosZ(), dir.z));
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
					Block *block = chunk->getBlockAt(pos.x - chunkX, pos.y, pos.z - chunkZ);
					if (block)
					{
						if (Main::getWindow()->isButtonDown(GLFW_MOUSE_BUTTON_LEFT))
						{
							chunk->destroyBlock(pos.x - chunkX, pos.y, pos.z - chunkZ);
							return;
						}
						else if (Main::getWindow()->isButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
						{
							int32_t newX = pos.x - chunkX;
							int32_t newY = pos.y;
							int32_t newZ = pos.z - chunkZ;
							if (face == BLOCK_FACE_LEFT)
								newX -= 1;
							else if (face == BLOCK_FACE_RIGHT)
								newX += 1;
							else if (face == BLOCK_FACE_FRONT)
								newZ += 1;
							else if (face == BLOCK_FACE_BACK)
								newZ -= 1;
							else if (face == BLOCK_FACE_UP)
								newY += 1;
							else if (face == BLOCK_FACE_DOWN)
								newY -= 1;
							Chunk *newChunk = chunk;
							if (newX < 0)
							{
								newChunk = chunk->getChunkXLess();
								newX = CHUNK_WIDTH - 1;
							}
							else if (newX >= CHUNK_WIDTH)
							{
								newChunk = chunk->getChunkXMore();
								newX = 0;
							}
							else if (newY < 0)
							{
								return;
							}
							else if (newY >= CHUNK_HEIGHT)
							{
								return;
							}
							else if (newZ < 0)
							{
								newChunk = chunk->getChunkZLess();
								newZ = CHUNK_WIDTH - 1;
							}
							else if (newZ >= CHUNK_WIDTH)
							{
								newChunk = chunk->getChunkZMore();
								newZ = 0;
							}
							if (newChunk->getBlockAt(newX, newY, newZ))
								return;
							newChunk->addBlock(newX, newY, newZ, 2);
							return;
						}
						this->found = true;
						this->face = face;
						this->x = pos.x;
						this->y = pos.y;
						this->z = pos.z;
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
