#include "DroppedBlock.h"
#include "World/ChunkTessellator.h"
#include "Blocks/Blocks.h"
#include "World/World.h"
#include "Main.h"

#define DROPPED_SIZE .1

extern int64_t nanotime;

using librender::Vec2;

namespace voxel
{

	DroppedBlock::DroppedBlock(World &world, Chunk *chunk, uint8_t type, Vec3 pos, Vec3 vel)
	: Entity(world, chunk)
	, created(nanotime)
	, number(1)
	, type(type)
	{
		Block *block = Blocks::getBlock(type);
		if (!block)
		{
			this->deleted = true;
			return;
		}
		setSize(Vec3(.3, .5, .3));
		setPos(pos);
		this->posDst = vel;
		ChunkTessellator tessellator;
		float tmp[24];
		for (uint8_t i = 0; i < 24; ++i)
			tmp[i] = ChunkBlock::getLightValue(0xf);
		block->draw(this->chunk, Vec3(0, 0, 0), tessellator, 0xff, tmp);
		this->positionBuffer.setData(GL_ARRAY_BUFFER, tessellator.positions.data(), sizeof(Vec3) * tessellator.positions.size(), GL_STATIC_DRAW);
		this->indiceBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, tessellator.indices.data(), sizeof(GLuint) * tessellator.indices.size(), GL_STATIC_DRAW);
		this->colorBuffer.setData(GL_ARRAY_BUFFER, tessellator.colors.data(), sizeof(Vec3) * tessellator.colors.size(), GL_STATIC_DRAW);
		this->uvBuffer.setData(GL_ARRAY_BUFFER, tessellator.uvs.data(), sizeof(Vec2) * tessellator.uvs.size(), GL_STATIC_DRAW);
		this->indicesNb = tessellator.indices.size();
		this->created = nanotime;
	}

	void DroppedBlock::draw()
	{
		Main::getDroppedShader().program->use();
		Main::getTerrain()->bind();
		Vec3 pos = getRealPos();
		Mat4 model(Mat4::translate(Mat4(1), pos));
		model = Mat4::translate(model, Vec3(0, std::sin((nanotime + this->created) / 500000000.) * .1, 0));
		model = Mat4::scale(model, Vec3(.2, .2, .2));
		model = Mat4::rotateY(model, (nanotime + this->created) / 1000000000.);
		model = Mat4::translate(model, Vec3(-.5, -.5, -.5));
		Mat4 mvp = this->world.getPlayer().getViewProjMat() * model;
		Main::getDroppedShader().vLocation.setMat4f(this->world.getPlayer().getViewMat());
		Main::getDroppedShader().mLocation.setMat4f(model);
		Main::getDroppedShader().mvpLocation.setMat4f(mvp);
		Main::getDroppedShader().vertexUVLocation.setVertexBuffer(this->uvBuffer, 2, GL_FLOAT);
		Main::getDroppedShader().vertexColorLocation.setVertexBuffer(this->colorBuffer, 3, GL_FLOAT);
		Main::getDroppedShader().vertexPositionLocation.setVertexBuffer(this->positionBuffer, 3, GL_FLOAT);
		this->indiceBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glEnable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, this->indicesNb, GL_UNSIGNED_INT, nullptr);
	}

}
