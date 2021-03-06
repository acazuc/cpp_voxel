#include "Clouds.h"
#include "World/World.h"
#include "Debug.h"
#include "Main.h"
#include <libformat/PNG.h>

#define CLOUD_Y 128
#define CLOUD_WIDTH 12
#define CLOUD_HEIGHT 4
#define DISPLAY_DISTANCE 16 * 16
#define FRONT_COLOR .9f
#define BACK_COLOR .9f
#define LEFT_COLOR .8f
#define RIGHT_COLOR .8f
#define UP_COLOR 1.f
#define DOWN_COLOR .7f

extern int64_t nanotime;

namespace voxel
{

	Clouds::Clouds(World &world)
	: world(world)
	{
		char *data;
		uint32_t width;
		uint32_t height;
		LOG("loading clouds.png");
		if (!libformat::PNG::read("data/textures/clouds.png", data, width, height))
			ERROR("Failed to read clouds.png");
		for (uint32_t i = 0; i < width * height; ++i)
			this->parts.push_back(data[i + 3] < 0);
		this->partsWidth = width;
		this->partsHeight = height;
		delete[] (data);
		rebuild();
	}

	Clouds::~Clouds()
	{
	}

	void Clouds::tick()
	{
		float offsetDiff = (-nanotime / 1000000000.) - this->lastXOffset;
		float playerXDiff = this->world.getPlayer().getPos().x - this->lastPlayerX;
		float playerZDiff = this->world.getPlayer().getPos().z - this->lastPlayerZ;
		float playerDiff = sqrt(playerXDiff * playerXDiff + playerZDiff * playerZDiff);
		if (offsetDiff + playerDiff > CLOUD_WIDTH * 3 || offsetDiff + playerDiff < -CLOUD_WIDTH * 3)
			rebuild();
	}

	void Clouds::draw()
	{
		Main::getCloudsShader().program->use();
		Mat4 model = Mat4::translate(Mat4(1), Vec3(-nanotime / 1000000000., CLOUD_Y, 0));
		Mat4 mvp = this->world.getPlayer().getViewProjMat() * model;
		Main::getCloudsShader().mvpLocation.setMat4f(mvp);
		Main::getCloudsShader().mLocation.setMat4f(model);
		Main::getCloudsShader().vLocation.setMat4f(this->world.getPlayer().getViewMat());
		Main::getCloudsShader().fogColorLocation.setVec4f(Main::getSkyColor());
		Main::getCloudsShader().vertexPositionLocation.setVertexBuffer(this->positionBuffer, 3, GL_FLOAT);
		Main::getCloudsShader().vertexColorLocation.setVertexBuffer(this->colorBuffer, 3, GL_FLOAT);
		this->indicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		glColorMask(false, false, false, false);
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_SHORT, nullptr);
		glColorMask(true, true, true, true);
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_SHORT, nullptr);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

	void Clouds::rebuild()
	{
		this->lastXOffset = -nanotime / 1000000000.;
		this->lastPlayerX = this->world.getPlayer().getPos().x;
		this->lastPlayerZ = this->world.getPlayer().getPos().z;
		std::vector<Vec3> positions;
		std::vector<Vec3> colors;
		std::vector<GLushort> indices;
		this->verticesNb = 0;
		int32_t baseX = this->world.getPlayer().getPos().x - DISPLAY_DISTANCE - CLOUD_WIDTH * 2 + nanotime / 1000000000.;
		int32_t endX = this->world.getPlayer().getPos().x + DISPLAY_DISTANCE + CLOUD_WIDTH * 2 + nanotime / 1000000000.;
		int32_t baseY = this->world.getPlayer().getPos().z - DISPLAY_DISTANCE - CLOUD_WIDTH * 2;
		int32_t endY = this->world.getPlayer().getPos().z + DISPLAY_DISTANCE + CLOUD_WIDTH * 2;
		baseX -= baseX % CLOUD_WIDTH;
		endX -= endX % CLOUD_WIDTH;
		baseY -= baseY % CLOUD_WIDTH;
		endY -= endY % CLOUD_WIDTH;
		for (int32_t x = baseX; x <= endX; x += CLOUD_WIDTH)
		{
			for (int32_t y = baseY; y <= endY; y += CLOUD_WIDTH)
			{
				int32_t relX = x / CLOUD_WIDTH;
				int32_t relY = y / CLOUD_WIDTH;
				if (!isPartFilled(relX, relY))
					continue;
				drawFaceUp(positions, colors, indices, x, y);
				drawFaceDown(positions, colors, indices, x, y);
				this->verticesNb += 12;
				if (!isPartFilled(relX - 1, relY))
				{
					drawFaceLeft(positions, colors, indices, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX + 1, relY))
				{
					drawFaceRight(positions, colors, indices, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX, relY - 1))
				{
					drawFaceBack(positions, colors, indices, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX, relY + 1))
				{
					drawFaceFront(positions, colors, indices, x, y);
					this->verticesNb += 6;
				}
			}
		}
		this->positionBuffer.setData(GL_ARRAY_BUFFER, positions.data(), positions.size() * sizeof(*positions.data()), GL_DYNAMIC_DRAW);
		this->indicesBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(*indices.data()), GL_DYNAMIC_DRAW);
		this->colorBuffer.setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(*colors.data()), GL_DYNAMIC_DRAW);
	}

	bool Clouds::isPartFilled(int32_t x, int32_t y)
	{
		x %= this->partsWidth;
		y %= this->partsHeight;
		if (x < 0)
			x += this->partsWidth;
		if (y < 0)
			y += this->partsHeight;
		return this->parts[y * this->partsWidth + x];
	}

	void Clouds::drawFaceUp(std::vector<Vec3> &positions, std::vector<Vec3> &colors, std::vector<GLushort> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = positions.size();
		positions.push_back(Vec3(x              , CLOUD_HEIGHT, y));
		positions.push_back(Vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		positions.push_back(Vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		positions.push_back(Vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(Vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		colors.push_back(Vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		colors.push_back(Vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		colors.push_back(Vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceDown(std::vector<Vec3> &positions, std::vector<Vec3> &colors, std::vector<GLushort> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = positions.size();
		positions.push_back(Vec3(x              , 0, y));
		positions.push_back(Vec3(x + CLOUD_WIDTH, 0, y));
		positions.push_back(Vec3(x              , 0, y + CLOUD_WIDTH));
		positions.push_back(Vec3(x + CLOUD_WIDTH, 0, y + CLOUD_WIDTH));
		colors.push_back(Vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		colors.push_back(Vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		colors.push_back(Vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		colors.push_back(Vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceLeft(std::vector<Vec3> &positions, std::vector<Vec3> &colors, std::vector<GLushort> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = positions.size();
		positions.push_back(Vec3(x, 0           , y));
		positions.push_back(Vec3(x, 0           , y + CLOUD_WIDTH));
		positions.push_back(Vec3(x, CLOUD_HEIGHT, y));
		positions.push_back(Vec3(x, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(Vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		colors.push_back(Vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		colors.push_back(Vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		colors.push_back(Vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceRight(std::vector<Vec3> &positions, std::vector<Vec3> &colors, std::vector<GLushort> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = positions.size();
		positions.push_back(Vec3(x + CLOUD_WIDTH, 0           , y));
		positions.push_back(Vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		positions.push_back(Vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		positions.push_back(Vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(Vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		colors.push_back(Vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		colors.push_back(Vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		colors.push_back(Vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceFront(std::vector<Vec3> &positions, std::vector<Vec3> &colors, std::vector<GLushort> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = positions.size();
		positions.push_back(Vec3(x              , 0           , y + CLOUD_WIDTH));
		positions.push_back(Vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		positions.push_back(Vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		positions.push_back(Vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(Vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		colors.push_back(Vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		colors.push_back(Vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		colors.push_back(Vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceBack(std::vector<Vec3> &positions, std::vector<Vec3> &colors, std::vector<GLushort> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = positions.size();
		positions.push_back(Vec3(x              , 0           , y));
		positions.push_back(Vec3(x              , CLOUD_HEIGHT, y));
		positions.push_back(Vec3(x + CLOUD_WIDTH, 0           , y));
		positions.push_back(Vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		colors.push_back(Vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		colors.push_back(Vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		colors.push_back(Vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		colors.push_back(Vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

}
