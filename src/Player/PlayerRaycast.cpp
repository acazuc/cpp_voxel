#include "PlayerRaycast.h"
#include "Entities/DroppedBlock.h"
#include "Blocks/Blocks.h"
#include "World/World.h"
#include "Debug.h"
#include "Main.h"
#include <cstring>

#define OFFSET .002
#define BREAK_OFFSET .002
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
			Vec3 vertexes[24];
			//Front
			vertexes[0]  = Vec3(0         , 0         , BLOCK_SIZE + BREAK_OFFSET);
			vertexes[1]  = Vec3(BLOCK_SIZE, 0         , BLOCK_SIZE + BREAK_OFFSET);
			vertexes[2]  = Vec3(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE + BREAK_OFFSET);
			vertexes[3]  = Vec3(0         , BLOCK_SIZE, BLOCK_SIZE + BREAK_OFFSET);
			//Back
			vertexes[4]  = Vec3(0         , 0         ,             -BREAK_OFFSET);
			vertexes[5]  = Vec3(BLOCK_SIZE, 0         ,             -BREAK_OFFSET);
			vertexes[6]  = Vec3(BLOCK_SIZE, BLOCK_SIZE,             -BREAK_OFFSET);
			vertexes[7]  = Vec3(0         , BLOCK_SIZE,             -BREAK_OFFSET);
			//Left
			vertexes[8]  = Vec3(0           -BREAK_OFFSET, 0         , 0         );
			vertexes[9]  = Vec3(0           -BREAK_OFFSET, BLOCK_SIZE, 0         );
			vertexes[10] = Vec3(0           -BREAK_OFFSET, BLOCK_SIZE, BLOCK_SIZE);
			vertexes[11] = Vec3(0           -BREAK_OFFSET, 0         , BLOCK_SIZE);
			//Right
			vertexes[12] = Vec3(BLOCK_SIZE + BREAK_OFFSET, 0         , 0         );
			vertexes[13] = Vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE, 0         );
			vertexes[14] = Vec3(BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE, BLOCK_SIZE);
			vertexes[15] = Vec3(BLOCK_SIZE + BREAK_OFFSET, 0         , BLOCK_SIZE);
			//Up
			vertexes[16] = Vec3(0         , BLOCK_SIZE + BREAK_OFFSET, 0         );
			vertexes[17] = Vec3(BLOCK_SIZE, BLOCK_SIZE + BREAK_OFFSET, 0         );
			vertexes[18] = Vec3(BLOCK_SIZE, BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE);
			vertexes[19] = Vec3(0         , BLOCK_SIZE + BREAK_OFFSET, BLOCK_SIZE);
			//Down
			vertexes[20] = Vec3(0         ,             -BREAK_OFFSET, 0         );
			vertexes[21] = Vec3(BLOCK_SIZE,             -BREAK_OFFSET, 0         );
			vertexes[22] = Vec3(BLOCK_SIZE,             -BREAK_OFFSET, BLOCK_SIZE);
			vertexes[23] = Vec3(0         ,             -BREAK_OFFSET, BLOCK_SIZE);
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
		Mat4 model = Mat4::translate(Mat4(1), this->pos);
		Mat4 mvp = this->player.getViewProjMat() * model;
		Main::getFocusedShader().mvpLocation->setMat4f(mvp);
		Main::getFocusedShader().vertexesLocation->setVertexBuffer(this->hoverVertexesBuffer);
		glLineWidth(2);
		glDrawArrays(GL_LINES, 0, 24);
		if (this->found && this->doneTicks != 0)
		{
			Main::getTerrain()->bind();
			Main::getBreakShader().program->use();
			Mat4 model = Mat4::translate(Mat4(1), this->pos);
			Mat4 mvp = this->player.getViewProjMat() * model;
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
		Vec3 org(this->player.getPos());
		org.y += .72;
		Vec3 pos(std::floor(this->player.getPos().x), std::floor(this->player.getPos().y + .72), std::floor(this->player.getPos().z));
		Vec3 dir((Vec4(0, 0, -1, 0) * this->player.getViewMat()).xyz());
		Vec3 step(signum(dir.x), signum(dir.y), signum(dir.z));
		Vec3 max(intbound(this->player.getPos().x, dir.x), intbound(this->player.getPos().y + .72, dir.y), intbound(this->player.getPos().z, dir.z));
		Vec3 delta(step.x / dir.x, step.y / dir.y, step.z / dir.z);
		float radius = PICK_DISTANCE;
		uint8_t face = 0;
		int32_t oldChunkX = -1;
		int32_t oldChunkZ = -1;
		Chunk *chunk = NULL;
		while (true)
		{
			if (pos.y >= 0 && pos.y < CHUNK_HEIGHT)
			{
				int32_t chunkX = World::getChunkCoord(pos.x);
				int32_t chunkZ = World::getChunkCoord(pos.z);
				if (chunkX != oldChunkX || chunkZ != oldChunkZ)
				{
					oldChunkX = chunkX;
					oldChunkZ = chunkZ;
					chunk = this->player.getWorld().getChunk(chunkX, chunkZ);
				}
				if (!chunk)
					goto nextStep;
				Vec3 relative(pos.x - chunkX, pos.y, pos.z - chunkZ);
				ChunkBlock *block = chunk->getBlock(relative.x, relative.y, relative.z);
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
				Vec3 interPos = org + dir * t;
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

	void PlayerRaycast::onLeftClick(Chunk *chunk, ChunkBlock *block, Vec3 &relative, Vec3 &pos)
	{
		Block *blockModel = Blocks::getBlock(block->getType());
		if (!blockModel)
			return;
		this->todoTicks = 20 * blockModel->getHardness();
		buildBreakTexCoords();
		uint8_t light = chunk->getLight(relative.x, relative.y, relative.z);
		light = std::max(light, chunk->getWorld().getLight(pos.x - 1, pos.y, pos.z));
		light = std::max(light, chunk->getWorld().getLight(pos.x + 1, pos.y, pos.z));
		light = std::max(light, chunk->getWorld().getLight(pos.x, pos.y - 1, pos.z));
		light = std::max(light, chunk->getWorld().getLight(pos.x, pos.y + 1, pos.z));
		light = std::max(light, chunk->getWorld().getLight(pos.x, pos.y, pos.z - 1));
		light = std::max(light, chunk->getWorld().getLight(pos.x, pos.y, pos.z + 1));
		if (this->doneTicks > 20 * blockModel->getHardness())
		{
			this->found = false;
			float texX = blockModel->getDestroyTexX();
			float texY = blockModel->getDestroyTexY();
			int32_t nb = 4;
			for (int32_t x = 0; x < nb; ++x)
			{
				for (int32_t y = 0; y < nb; ++y)
				{
					for (int32_t z = 0; z < nb; ++z)
					{
						float tmpx = (x + .5) / nb;
						float tmpy = (y + .5) / nb;
						float tmpz = (z + .5) / nb;
						Vec3 pos2(pos.x + tmpx, pos.y + tmpy, pos.z + tmpz);
						pos.x += std::rand() * .1 / RAND_MAX - .05;
						pos.y += std::rand() * .1 / RAND_MAX - .05;
						pos.z += std::rand() * .1 / RAND_MAX - .05;
						Vec2 size((std::rand() * .5 / RAND_MAX + .5) * .2, 0);
						size.y = size.x;
						Vec3 dir(tmpx - .5, tmpy - .5, tmpz - .5);
						dir.x += (std::rand() * 2. / RAND_MAX - 1.);
						dir.y += (std::rand() * 2. / RAND_MAX - 1.);
						dir.z += (std::rand() * 2. / RAND_MAX - 1.);
						dir.normalize();
						dir *= (std::rand() / (float)RAND_MAX + std::rand() / (float)RAND_MAX + 1) * .3 * .4 * .4;
						dir.y += .1f;
						Vec2 uv(texX, texY);
						uv.x += std::rand() * 1. / 16 * 14 / 16 / RAND_MAX;
						uv.y += std::rand() * 1. / 16 * 14 / 16 / RAND_MAX;
						Vec2 uvSize(1. / 16 / 8, 1. / 16 / 8);
						Particle *particle = new Particle(this->player.getWorld(), chunk, pos2, size, dir, uv, uvSize, light);
						chunk->getParticlesManager().addParticle(particle);
					}
				}
			}
			Vec3 position(pos + .5f);
			Vec3 velocity(std::rand() * .2 / RAND_MAX - .1, .25 + std::rand() * .1 / RAND_MAX, std::rand() * .2 / RAND_MAX - .1);
			DroppedBlock *tmp = new DroppedBlock(this->player.getWorld(), chunk, block->getType(), position, velocity);
			chunk->getEntitiesManager().addEntity(tmp);
			chunk->destroyBlock(relative.x, relative.y, relative.z);
		}
	}

	void PlayerRaycast::onRightClick(Chunk *chunk, Vec3 &relative)
	{
		Vec3 newPos(relative);
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
		newChunk->setBlockIfReplaceable(newPos.x, newPos.y, newPos.z, 2);
	}

	void PlayerRaycast::buildBreakTexCoords()
	{
		Vec2 texCoords[24];
		uint8_t pos = this->doneTicks * 9. / this->todoTicks;
		Vec2 org(pos / 16., 15. / 16);
		Vec2 dst(org);
		dst += 1. / 16;
		//Front
		texCoords[0]  = Vec2(org.x, dst.y);
		texCoords[1]  = Vec2(dst.x, dst.y);
		texCoords[2]  = Vec2(dst.x, org.y);
		texCoords[3]  = Vec2(org.x, org.y);
		//Back
		texCoords[4]  = Vec2(dst.x, dst.y);
		texCoords[5]  = Vec2(org.x, dst.y);
		texCoords[6]  = Vec2(org.x, org.y);
		texCoords[7]  = Vec2(dst.x, org.y);
		//Left
		texCoords[8]  = Vec2(org.x, dst.y);
		texCoords[9]  = Vec2(org.x, org.y);
		texCoords[10] = Vec2(dst.x, org.y);
		texCoords[11] = Vec2(dst.x, dst.y);
		//Right
		texCoords[12] = Vec2(dst.x, dst.y);
		texCoords[13] = Vec2(dst.x, org.y);
		texCoords[14] = Vec2(org.x, org.y);
		texCoords[15] = Vec2(org.x, dst.y);
		//Up
		texCoords[16] = Vec2(org.x, dst.y);
		texCoords[17] = Vec2(dst.x, dst.y);
		texCoords[18] = Vec2(dst.x, org.y);
		texCoords[19] = Vec2(org.x, org.y);
		//Down
		texCoords[20] = Vec2(org.x, org.y);
		texCoords[21] = Vec2(dst.x, org.y);
		texCoords[22] = Vec2(dst.x, dst.y);
		texCoords[23] = Vec2(org.x, dst.y);
		this->breakTexCoordsBuffer.setData(GL_ARRAY_BUFFER, texCoords, sizeof(texCoords), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
	}

	void PlayerRaycast::buildHoverVertexes(AABB aabb)
	{
		Vec3 vertexes[24];
		vertexes[0]  = Vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[1]  = Vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[2]  = Vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[3]  = Vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[4]  = Vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[5]  = Vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[6]  = Vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[7]  = Vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[8]  = Vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[9]  = Vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[10] = Vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[11] = Vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[12] = Vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[13] = Vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[14] = Vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[15] = Vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[16] = Vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[17] = Vec3(aabb.getP0().x - OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[18] = Vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP1().z + OFFSET);
		vertexes[19] = Vec3(aabb.getP1().x + OFFSET, aabb.getP0().y - OFFSET, aabb.getP0().z - OFFSET);
		vertexes[20] = Vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[21] = Vec3(aabb.getP0().x - OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		vertexes[22] = Vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP1().z + OFFSET);
		vertexes[23] = Vec3(aabb.getP1().x + OFFSET, aabb.getP1().y + OFFSET, aabb.getP0().z - OFFSET);
		this->hoverVertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes, sizeof(vertexes), GL_FLOAT, 3, GL_STATIC_DRAW);
	}

}
