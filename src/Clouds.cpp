#include "Clouds.h"
#include "Debug.h"
#include "World.h"
#include "Utils/System.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <libformat/PNG.h>

#define CLOUD_Y 128
#define CLOUD_WIDTH 12
#define CLOUD_HEIGHT 4
#define DISPLAY_DISTANCE 16 * 16

extern int64_t nanotime;

namespace voxel
{

	Clouds::Clouds(World &world)
	: world(world)
	{
		char *data;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("data/textures/clouds.png", data, width, height))
			ERROR("Failed to read clouds.png");
		for (uint32_t i = 0; i < width * height; ++i)
			this->parts.push_back(data[i * 4 + 3] < 0);
		this->partsWidth = width;
		this->partsHeight = height;
		delete[] (data);
		rebuild();
	}

	Clouds::~Clouds()
	{
		//Empty
	}

	void Clouds::draw()
	{
		float offsetDiff = (-nanotime / 1000000000.) - this->lastXOffset;
		float playerXDiff = this->world.getPlayer().getPosX() - this->lastPlayerX;
		float playerZDiff = this->world.getPlayer().getPosZ() - this->lastPlayerZ;
		float playerDiff = sqrt(playerXDiff * playerXDiff + playerZDiff * playerZDiff);
		if (offsetDiff + playerDiff > CLOUD_WIDTH || offsetDiff + playerDiff < -CLOUD_WIDTH)
			rebuild();
		Main::getCloudsShader().program->use();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(-nanotime / 1000000000., CLOUD_Y, 0));
		glm::mat4 mvp = this->world.getPlayer().getProjMat() * this->world.getPlayer().getViewMat() * model;
		Main::getCloudsShader().mvpLocation->setMat4f(mvp);
		Main::getCloudsShader().mLocation->setMat4f(model);
		Main::getBlocksShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		Main::getCloudsShader().vertexesLocation->setDataBuffer(this->vertexesBuffer);
		Main::getCloudsShader().colorsLocation->setDataBuffer(this->colorsBuffer);
		glDrawArrays(GL_TRIANGLES, 0, this->verticesNb);
	}

	void Clouds::rebuild()
	{
		this->lastXOffset = -nanotime / 1000000000.;
		this->lastPlayerX = this->world.getPlayer().getPosX();
		this->lastPlayerZ = this->world.getPlayer().getPosZ();
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		this->verticesNb = 0;
		int32_t baseX = this->world.getPlayer().getPosX() - DISPLAY_DISTANCE;
		int32_t endX = this->world.getPlayer().getPosX() + DISPLAY_DISTANCE;
		int32_t baseY = this->world.getPlayer().getPosZ() - DISPLAY_DISTANCE;
		int32_t endY = this->world.getPlayer().getPosZ() + DISPLAY_DISTANCE;
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
				drawFaceUp(vertexes, colors, x, y);
				drawFaceDown(vertexes, colors, x, y);
				this->verticesNb += 12;
				if (!isPartFilled(relX - 1, relY))
				{
					drawFaceLeft(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX + 1, relY))
				{
					drawFaceRight(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX, relY - 1))
				{
					drawFaceBack(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX, relY + 1))
				{
					drawFaceFront(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
			}
		}
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes.data(), vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->colorsBuffer.setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
	}

	bool Clouds::isPartFilled(int32_t x, int32_t y)
	{
		x %= this->partsWidth;
		y %= this->partsHeight;
		if (x < 0)
			x += this->partsWidth;
		if (y < 0)
			y += this->partsHeight;
		return (this->parts[y * this->partsWidth + x]);
	}

	void Clouds::drawFaceUp(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y)
	{
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
	}

	void Clouds::drawFaceDown(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, int32_t x, int32_t y)
	{
		vertexes.push_back(glm::vec3(x              , 0, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0, y));
		vertexes.push_back(glm::vec3(x              , 0, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x              , 0, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0, y));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
	}

	void Clouds::drawFaceLeft(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, int32_t x, int32_t y)
	{
		vertexes.push_back(glm::vec3(x, 0           , y));
		vertexes.push_back(glm::vec3(x, 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x, 0           , y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

	void Clouds::drawFaceRight(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, int32_t x, int32_t y)
	{
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

	void Clouds::drawFaceFront(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, int32_t x, int32_t y)
	{
		vertexes.push_back(glm::vec3(x              , 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

	void Clouds::drawFaceBack(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, int32_t x, int32_t y)
	{
		vertexes.push_back(glm::vec3(x              , 0           , y));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

}
