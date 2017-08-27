#include "Clouds.h"
#include "Debug.h"
#include "World.h"
#include "Main.h"
#include <libformat/PNG.h>

#define CLOUD_Y 128
#define CLOUD_WIDTH 12
#define CLOUD_HEIGHT 4

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
		//
	}

	void Clouds::draw()
	{
		Main::getCloudsShader().program->use();
		glm::mat4 mvp = this->world.getPlayer().getProjMat() * this->world.getPlayer().getViewMat();
		Main::getCloudsShader().mvpLocation->setMat4f(mvp);
		Main::getCloudsShader().vertexesLocation->setDataBuffer(this->vertexesBuffer);
		Main::getCloudsShader().colorsLocation->setDataBuffer(this->colorsBuffer);
		glDrawArrays(GL_TRIANGLES, 0, this->verticesNb);
	}

	void Clouds::rebuild()
	{
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		this->verticesNb = 0;
		for (uint32_t x = 0; x < this->partsWidth; ++x)
		{
			for (uint32_t y = 0; y < this->partsHeight; ++y)
			{
				if (!isPartFilled(x, y))
					continue;
				drawFaceUp(vertexes, colors, x, y);
				drawFaceDown(vertexes, colors, x, y);
				this->verticesNb += 12;
				if (x == 0 || !isPartFilled(x - 1, y))
				{
					drawFaceLeft(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
				if (x == this->partsWidth - 1 || !isPartFilled(x + 1, y))
				{
					drawFaceRight(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
				if (y == 0 || !isPartFilled(x, y - 1))
				{
					drawFaceBack(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
				if (y == this->partsHeight - 1 || !isPartFilled(x, y + 1))
				{
					drawFaceFront(vertexes, colors, x, y);
					this->verticesNb += 6;
				}
			}
		}
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes.data(), vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->colorsBuffer.setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
	}

	bool Clouds::isPartFilled(uint32_t x, uint32_t y)
	{
		return (this->parts[y * this->partsWidth + x]);
	}

	void Clouds::drawFaceUp(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, uint32_t x, uint32_t y)
	{
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
		colors.push_back(glm::vec3(1, 1, 1));
	}

	void Clouds::drawFaceDown(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, uint32_t x, uint32_t y)
	{
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y, y * CLOUD_WIDTH));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
		colors.push_back(glm::vec3(.75, .75, .75));
	}

	void Clouds::drawFaceLeft(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, uint32_t x, uint32_t y)
	{
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH + CLOUD_WIDTH));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

	void Clouds::drawFaceRight(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, uint32_t x, uint32_t y)
	{
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

	void Clouds::drawFaceFront(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, uint32_t x, uint32_t y)
	{
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y               , y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH + CLOUD_WIDTH));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

	void Clouds::drawFaceBack(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> & colors, uint32_t x, uint32_t y)
	{
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y               , y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH + CLOUD_WIDTH, CLOUD_Y               , y * CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x * CLOUD_WIDTH              , CLOUD_Y + CLOUD_HEIGHT, y * CLOUD_WIDTH));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
		colors.push_back(glm::vec3(.9, .9, .9));
	}

}
