#include "Clouds.h"
#include "World/World.h"
#include "Debug.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <libformat/PNG.h>

#define CLOUD_Y 128
#define CLOUD_WIDTH 12
#define CLOUD_HEIGHT 4
#define DISPLAY_DISTANCE 16 * 32
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
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(-nanotime / 1000000000., CLOUD_Y, 0));
		glm::mat4 mvp = this->world.getPlayer().getViewProjMat() * model;
		Main::getCloudsShader().mvpLocation->setMat4f(mvp);
		Main::getCloudsShader().mLocation->setMat4f(model);
		Main::getCloudsShader().vLocation->setMat4f(this->world.getPlayer().getViewMat());
		Main::getCloudsShader().fogColorLocation->setVec4f(Main::getSkyColor());
		Main::getCloudsShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		Main::getCloudsShader().colorsLocation->setVertexBuffer(this->colorsBuffer);
		this->indicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glColorMask(false, false, false, false);
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_INT, (void*)0);
		glColorMask(true, true, true, true);
		glDepthFunc(GL_LEQUAL);
		glDrawElements(GL_TRIANGLES, this->verticesNb, GL_UNSIGNED_INT, (void*)0);
		glDepthFunc(GL_LESS);
	}

	void Clouds::rebuild()
	{
		this->lastXOffset = -nanotime / 1000000000.;
		this->lastPlayerX = this->world.getPlayer().getPos().x;
		this->lastPlayerZ = this->world.getPlayer().getPos().z;
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec3> colors;
		std::vector<GLuint> indices;
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
				drawFaceUp(vertexes, colors, indices, x, y);
				drawFaceDown(vertexes, colors, indices, x, y);
				this->verticesNb += 12;
				if (!isPartFilled(relX - 1, relY))
				{
					drawFaceLeft(vertexes, colors, indices, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX + 1, relY))
				{
					drawFaceRight(vertexes, colors, indices, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX, relY - 1))
				{
					drawFaceBack(vertexes, colors, indices, x, y);
					this->verticesNb += 6;
				}
				if (!isPartFilled(relX, relY + 1))
				{
					drawFaceFront(vertexes, colors, indices, x, y);
					this->verticesNb += 6;
				}
			}
		}
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes.data(), vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->indicesBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
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

	void Clouds::drawFaceUp(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		colors.push_back(glm::vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		colors.push_back(glm::vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		colors.push_back(glm::vec3(UP_COLOR, UP_COLOR, UP_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceDown(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(x              , 0, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0, y));
		vertexes.push_back(glm::vec3(x              , 0, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0, y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		colors.push_back(glm::vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		colors.push_back(glm::vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		colors.push_back(glm::vec3(DOWN_COLOR, DOWN_COLOR, DOWN_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceLeft(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(x, 0           , y));
		vertexes.push_back(glm::vec3(x, 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		colors.push_back(glm::vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		colors.push_back(glm::vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		colors.push_back(glm::vec3(LEFT_COLOR, LEFT_COLOR, LEFT_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceRight(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		colors.push_back(glm::vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		colors.push_back(glm::vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		colors.push_back(glm::vec3(RIGHT_COLOR, RIGHT_COLOR, RIGHT_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceFront(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(x              , 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y + CLOUD_WIDTH));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y + CLOUD_WIDTH));
		colors.push_back(glm::vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		colors.push_back(glm::vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		colors.push_back(glm::vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		colors.push_back(glm::vec3(FRONT_COLOR, FRONT_COLOR, FRONT_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

	void Clouds::drawFaceBack(std::vector<glm::vec3> &vertexes, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices, int32_t x, int32_t y)
	{
		GLuint currentIndice = vertexes.size();
		vertexes.push_back(glm::vec3(x              , 0           , y));
		vertexes.push_back(glm::vec3(x              , CLOUD_HEIGHT, y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, 0           , y));
		vertexes.push_back(glm::vec3(x + CLOUD_WIDTH, CLOUD_HEIGHT, y));
		colors.push_back(glm::vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		colors.push_back(glm::vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		colors.push_back(glm::vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		colors.push_back(glm::vec3(BACK_COLOR, BACK_COLOR, BACK_COLOR));
		indices.push_back(currentIndice + 0);
		indices.push_back(currentIndice + 1);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 3);
		indices.push_back(currentIndice + 2);
		indices.push_back(currentIndice + 1);
	}

}
