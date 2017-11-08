#include "PlayerRaycast.h"
#include "Entities/DroppedBlock.h"
#include "Blocks/Blocks.h"
#include "World/World.h"
#include "Debug.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

#define OFFSET .001
#define BREAK_OFFSET .0001
#define PICK_DISTANCE 500
#define PICK_EPSILON .001

namespace voxel
{

	PlayerRaycast::PlayerRaycast(Player &player)
	: player(player)
	, doneTicks(0)
	, found(false)
	{
		{
			glm::vec3 vertexes[24];
			//Front
			vertexes[0]  = glm::vec3(            -BREAK_OFFSET,             -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[1]  = glm::vec3(BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[2]  = glm::vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[3]  = glm::vec3(            -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			//Back
			vertexes[4]  = glm::vec3(            -BREAK_OFFSET,             -BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[5]  = glm::vec3(BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[6]  = glm::vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[7]  = glm::vec3(            -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET);
			//Left
			vertexes[8]  = glm::vec3(            -BREAK_OFFSET,             -BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[9]  = glm::vec3(            -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[10] = glm::vec3(            -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[11] = glm::vec3(            -BREAK_OFFSET,             -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			//Right
			vertexes[12] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[13] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[14] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[15] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			//Up
			vertexes[16] = glm::vec3(            -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[17] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[18] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[19] = glm::vec3(            -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			//Down
			vertexes[20] = glm::vec3(            -BREAK_OFFSET,             -BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[21] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET,             -BREAK_OFFSET);
			vertexes[22] = glm::vec3(BLOCK_SIZE + BREAK_OFFSET,             -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[23] = glm::vec3(            -BREAK_OFFSET,             -BREAK_OFFSET, BLOCK_SIZE + BREAK_OFFSET);
			this->breakVertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes, sizeof(vertexes), GL_FLOAT, 3, GL_STATIC_DRAW);
			GLuint indices[36];
			//Front
			indices[0]  = 0 * 4 + 0;
			indices[1]  = 0 * 4 + 1;
			indices[2]  = 0 * 4 + 3;
			indices[3]  = 0 * 4 + 2;
			indices[4]  = 0 * 4 + 3;
			indices[5]  = 0 * 4 + 1;
			//Back
			indices[6]  = 1 * 4 + 0;
			indices[7]  = 1 * 4 + 3;
			indices[8]  = 1 * 4 + 1;
			indices[9]  = 1 * 4 + 2;
			indices[10] = 1 * 4 + 1;
			indices[11] = 1 * 4 + 3;
			//Left
			indices[12] = 2 * 4 + 0;
			indices[13] = 2 * 4 + 3;
			indices[14] = 2 * 4 + 1;
			indices[15] = 2 * 4 + 2;
			indices[16] = 2 * 4 + 1;
			indices[17] = 2 * 4 + 3;
			//Right
			indices[18] = 3 * 4 + 0;
			indices[19] = 3 * 4 + 1;
			indices[20] = 3 * 4 + 3;
			indices[21] = 3 * 4 + 2;
			indices[22] = 3 * 4 + 3;
			indices[23] = 3 * 4 + 1;
			//Up
			indices[24] = 4 * 4 + 0;
			indices[25] = 4 * 4 + 1;
			indices[26] = 4 * 4 + 3;
			indices[27] = 4 * 4 + 2;
			indices[28] = 4 * 4 + 3;
			indices[29] = 4 * 4 + 1;
			//Down
			indices[24] = 4 * 4 + 0;
			indices[25] = 4 * 4 + 3;
			indices[26] = 4 * 4 + 1;
			indices[27] = 4 * 4 + 2;
			indices[28] = 4 * 4 + 1;
			indices[29] = 4 * 4 + 3;
			this->breakIndicesBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), GL_UNSIGNED_INT, 1, GL_STATIC_DRAW);
		}
	}

	PlayerRaycast::~PlayerRaycast()
	{
		//
	}

	void PlayerRaycast::tick()
	{
		if (this->found)
			this->doneTicks++;
		raycast();
	}

	void PlayerRaycast::draw()
	{
		if (!this->found)
			return;
		glDepthFunc(GL_LEQUAL);
		Main::getFocusedShader().program->use();
		glm::mat4 model(1);
		model = glm::translate(model, this->pos);
		glm::mat4 mvp = this->player.getProjMat() * this->player.getViewMat() * model;
		Main::getFocusedShader().mvpLocation->setMat4f(mvp);
		Main::getFocusedShader().vertexesLocation->setVertexBuffer(this->hoverVertexesBuffer);
		glLineWidth(2);
		glDrawArrays(GL_LINES, 0, 24);
		if (this->found && this->doneTicks != 0)
		{
			Main::getTerrain()->bind();
			Main::getBreakShader().program->use();
			glm::mat4 model(1);
			model = glm::translate(model, this->pos);
			glm::mat4 mvp = this->player.getProjMat() * this->player.getViewMat() * model;
			Main::getBreakShader().mvpLocation->setMat4f(mvp);
			Main::getBreakShader().mLocation->setMat4f(model);
			Main::getBreakShader().vLocation->setMat4f(this->player.getViewMat());
			Main::getBreakShader().fogColorLocation->setVec4f(Main::getSkyColor());
			Main::getBreakShader().texCoordsLocation->setVertexBuffer(this->breakTexCoordsBuffer);
			Main::getBreakShader().vertexesLocation->setVertexBuffer(this->breakVertexesBuffer);
			this->breakIndicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			glBlendEquation(GL_FUNC_ADD);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glDepthFunc(GL_LESS);
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
		glm::vec3 org(this->player.getPos());
		org.y += .72;
		glm::vec3 pos(std::floor(this->player.getPos().x), std::floor(this->player.getPos().y + .72), std::floor(this->player.getPos().z));
		glm::vec4 dir = glm::vec4(0, 0, -1, 0) * this->player.getViewMat();
		glm::vec3 step(signum(dir.x), signum(dir.y), signum(dir.z));
		glm::vec3 max(intbound(this->player.getPos().x, dir.x), intbound(this->player.getPos().y + .72, dir.y), intbound(this->player.getPos().z, dir.z));
		glm::vec3 delta(step.x / dir.x, step.y / dir.y, step.z / dir.z);
		float radius = PICK_DISTANCE;
		uint8_t face = 0;
		int32_t oldChunkX = -1;
		int32_t oldChunkZ = -1;
		Chunk *chunk = NULL;
		while (true)
		{
			if (pos.y >= 0 && pos.y < CHUNK_HEIGHT)
			{
				int32_t chunkX = std::floor(pos.x / CHUNK_WIDTH) * CHUNK_WIDTH;
				int32_t chunkZ = std::floor(pos.z / CHUNK_WIDTH) * CHUNK_WIDTH;
				if (chunkX != oldChunkX || chunkZ != oldChunkZ)
				{
					oldChunkX = chunkX;
					oldChunkZ = chunkZ;
					chunk = this->player.getWorld().getChunk(chunkX, chunkZ);
				}
				if (!chunk)
					goto nextStep;
				glm::vec3 relative(pos.x - chunkX, pos.y, pos.z - chunkZ);
				ChunkBlock *block = chunk->getBlock(glm::vec3(relative));
				if (!block || !block->getType())
					goto nextStep;
				Block *blockModel = Blocks::getBlock(block->getType());
				if (!blockModel || !blockModel->isFocusable())
					goto nextStep;
				float t = 0;
				AABB aabb = blockModel->getAABB();
				aabb.move(pos);
				if (!aabb.intersect(org, dir, t))
					goto nextStep;
				if (this->pos != pos)
					doneTicks = 0;
				glm::vec3 interPos = org + glm::vec3(dir) * t;
				float dst[6];
				dst[0] = std::abs(interPos.x - aabb.getP0().x);
				dst[1] = std::abs(interPos.x - aabb.getP1().x);
				dst[2] = std::abs(interPos.y - aabb.getP0().y);
				dst[3] = std::abs(interPos.y - aabb.getP1().y);
				dst[4] = std::abs(interPos.z - aabb.getP0().z);
				dst[5] = std::abs(interPos.z - aabb.getP1().z);
				float min = std::min(dst[0], std::min(dst[1], std::min(dst[2], std::min(dst[3], std::min(dst[4], dst[5])))));
				if (min == dst[0])
					this->face = BLOCK_FACE_LEFT;
				else if (min == dst[1])
					this->face = BLOCK_FACE_RIGHT;
				else if (min == dst[2])
					this->face = BLOCK_FACE_BACK;
				else if (min == dst[3])
					this->face = BLOCK_FACE_FRONT;
				else if (min == dst[4])
					this->face = BLOCK_FACE_UP;
				else if (min == dst[5])
					this->face = BLOCK_FACE_DOWN;
				else
					goto nextStep;
				buildHoverVertexes(blockModel->getAABB());
				this->face = face;
				this->pos = pos;
				this->found = true;
				if (Main::getWindow()->isButtonDown(GLFW_MOUSE_BUTTON_LEFT))
				{
					onLeftClick(chunk, block, relative, pos);
					return;
				}
				else
					doneTicks = 0;
				if (Main::getWindow()->isButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
				{
					onRightClick(chunk, relative);
					return;
				}
				return;
			}
nextStep:
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
		this->doneTicks = 0;
	}

	void PlayerRaycast::onLeftClick(Chunk *chunk, ChunkBlock *block, glm::vec3 &relative, glm::vec3 &pos)
	{
		Block *blockModel = Blocks::getBlock(block->getType());
		if (!blockModel)
			return;
		this->todoTicks = 20 * blockModel->getHardness();
		buildBreakTexCoords();
		uint8_t light = chunk->getLight(relative);
		if (this->doneTicks > 20 * blockModel->getHardness())
		{
			this->found = false;
			float texX = 0;//blockModel->getTexTopX();
			float texY = 0;//blockModel->getTexTopY();
			Chunk *chunk = this->player.getWorld().getChunk(std::floor(pos.x / CHUNK_WIDTH) * CHUNK_WIDTH, std::floor(pos.z / CHUNK_WIDTH) * CHUNK_WIDTH);
			if (!chunk)
				return;
			int32_t nb = 4;
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
						glm::vec2 size((std::rand() * .5 / RAND_MAX + .5) * .2, (std::rand() * .5 / RAND_MAX + .5) * .2);
						glm::vec3 dir(std::rand() * .2 / RAND_MAX - .1, std::rand() * .3 / RAND_MAX, std::rand() * .2 / RAND_MAX - .1);
						glm::vec2 uv(texX, texY);
						uv.x += std::rand() * 1. / 16 * 14 / 16 / RAND_MAX;
						uv.y += std::rand() * 1. / 16 * 14 / 16 / RAND_MAX;
						glm::vec2 uvSize(1. / 16 / 8, 1. / 16 / 8);
						Particle *particle = new Particle(this->player.getWorld(), pos2, size, dir, uv, uvSize, light);
						chunk->getParticlesManager().addParticle(particle);
					}
				}
			}
			glm::vec3 position(pos + .4f + std::rand() * .2f / RAND_MAX);
			glm::vec3 velocity(std::rand() * .2 / RAND_MAX - .1, .25 + std::rand() * .25 / RAND_MAX, std::rand() * .2 / RAND_MAX - .1);
			DroppedBlock *tmp = new DroppedBlock(this->player.getWorld(), chunk, block->getType(), position, velocity);
			chunk->getEntitiesManager().addEntity(tmp);
			chunk->destroyBlock(relative);
		}
	}

	void PlayerRaycast::onRightClick(Chunk *chunk, glm::vec3 &relative)
	{
		glm::vec3 newPos(relative);
		if (this->face == BLOCK_FACE_LEFT)
			newPos.x -= 1;
		else if (this->face == BLOCK_FACE_RIGHT)
			newPos.x += 1;
		else if (this->face == BLOCK_FACE_FRONT)
			newPos.z += 1;
		else if (this->face == BLOCK_FACE_BACK)
			newPos.z -= 1;
		else if (this->face == BLOCK_FACE_UP)
			newPos.y += 1;
		else if (this->face == BLOCK_FACE_DOWN)
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
		newChunk->setBlockIfReplaceable(newPos, 2);
	}

	void PlayerRaycast::buildBreakTexCoords()
	{
		glm::vec2 texCoords[24];
		uint8_t pos = this->doneTicks * 9. / this->todoTicks;
		glm::vec2 org(pos / 16., 15. / 16);
		glm::vec2 dst(org);
		dst += 1. / 16;
		//Front
		texCoords[0]  = glm::vec2(org.x, dst.y);
		texCoords[1]  = glm::vec2(dst.x, dst.y);
		texCoords[2]  = glm::vec2(dst.x, org.y);
		texCoords[3]  = glm::vec2(org.x, org.y);
		//Back
		texCoords[4]  = glm::vec2(dst.x, dst.y);
		texCoords[5]  = glm::vec2(org.x, dst.y);
		texCoords[6]  = glm::vec2(org.x, org.y);
		texCoords[7]  = glm::vec2(dst.x, org.y);
		//Left
		texCoords[8]  = glm::vec2(org.x, dst.y);
		texCoords[9]  = glm::vec2(org.x, org.y);
		texCoords[10] = glm::vec2(dst.x, org.y);
		texCoords[11] = glm::vec2(dst.x, dst.y);
		//Right
		texCoords[12] = glm::vec2(dst.x, dst.y);
		texCoords[13] = glm::vec2(dst.x, org.y);
		texCoords[14] = glm::vec2(org.x, org.y);
		texCoords[15] = glm::vec2(org.x, dst.y);
		//Up
		texCoords[16] = glm::vec2(org.x, dst.y);
		texCoords[17] = glm::vec2(dst.x, dst.y);
		texCoords[18] = glm::vec2(dst.x, org.y);
		texCoords[19] = glm::vec2(org.x, org.y);
		//Down
		texCoords[20] = glm::vec2(org.x, org.y);
		texCoords[21] = glm::vec2(dst.x, org.y);
		texCoords[22] = glm::vec2(dst.x, dst.y);
		texCoords[23] = glm::vec2(org.x, dst.y);
		this->breakTexCoordsBuffer.setData(GL_ARRAY_BUFFER, texCoords, sizeof(texCoords), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
	}

	void PlayerRaycast::buildHoverVertexes(AABB aabb)
	{
		glm::vec3 vertexes[24];
		vertexes[0]  = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[1]  = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[2]  = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[3]  = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[4]  = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[5]  = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[6]  = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[7]  = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[8]  = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[9]  = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[10] = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[11] = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[12] = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[13] = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[14] = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[15] = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[16] = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[17] = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[18] = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[19] = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[20] = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[21] = glm::vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[22] = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[23] = glm::vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		this->hoverVertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes, sizeof(vertexes), GL_FLOAT, 3, GL_STATIC_DRAW);
	}

}
