#include "Skybox.h"
#include "World.h"
#include "Debug.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>
#include <libformat/PNG.h>

extern int64_t nanotime;

namespace voxel
{

	Skybox::Skybox(World &world)
	: world(world)
	{
		uint8_t *data;
		char *fakeData;
		uint32_t width;
		uint32_t height;
		if (!libformat::PNG::read("data/textures/sun.png", fakeData, width, height))
			ERROR("Failed to read sun.png");
		data = reinterpret_cast<uint8_t*>(fakeData);
		for (uint32_t i = 0; i < width * height; ++i)
		{
			uint8_t red = data[i * 4 + 0];
			uint8_t green = data[i * 4 + 1];
			uint8_t blue = data[i * 4 + 2];
			uint8_t max = std::max(red, std::max(green, blue));
			float factor = 255. / max;
			data[i * 4 + 0] *= factor;
			data[i * 4 + 1] *= factor;
			data[i * 4 + 2] *= factor;
			data[i * 4 + 3] = max;
		}
		this->sun = new Texture(fakeData, width, height);
		this->sun->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		delete[] (data);
		if (!libformat::PNG::read("data/textures/moon_phases.png", fakeData, width, height))
			ERROR("Failed to read moon_phases.png");
		data = reinterpret_cast<uint8_t*>(fakeData);
		for (uint32_t i = 0; i < width * height; ++i)
		{
			uint8_t red = data[i * 4 + 0];
			uint8_t green = data[i * 4 + 1];
			uint8_t blue = data[i * 4 + 2];
			uint8_t max = std::max(red, std::max(green, blue));
			if (max != 0)
			{
				data[i * 4 + 0] = ((uint32_t)data[i * 4 + 0]) * 255 / max;
				data[i * 4 + 1] = ((uint32_t)data[i * 4 + 1]) * 255 / max;
				data[i * 4 + 2] = ((uint32_t)data[i * 4 + 2]) * 255 / max;
			}
			data[i * 4 + 3] = max;
		}
		this->moon = new Texture(fakeData, width, height);
		this->moon->bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		delete[] (data);
		initBuffers();
	}

	Skybox::~Skybox()
	{
		delete (this->sun);
		delete (this->moon);
	}

	void Skybox::draw()
	{
		Main::getSkyboxShader().program->use();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(this->world.getPlayer().getPosX(), this->world.getPlayer().getPosY(), this->world.getPlayer().getPosZ()));
		model = glm::rotate(model, glm::vec2(glm::radians(-nanotime / 1000000000. / 60 / 20 * 360), 0).x, glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(1.5, 1.5, 1));
		glm::mat4 mvp = this->world.getPlayer().getProjMat() * this->world.getPlayer().getViewMat() * model;
		glBindTexture(GL_TEXTURE_2D, sun->getId());
		Main::getSkyboxShader().mvpLocation->setMat4f(mvp);
		Main::getSkyboxShader().texCoordsLocation->setVertexBuffer(this->sunTexCoordsBuffer);
		Main::getSkyboxShader().vertexesLocation->setVertexBuffer(this->sunVertexesBuffer);
		Main::getSkyboxShader().colorsLocation->setVertexBuffer(this->sunColorsBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, moon->getId());
		Main::getSkyboxShader().texCoordsLocation->setVertexBuffer(this->moonTexCoordsBuffer);
		Main::getSkyboxShader().vertexesLocation->setVertexBuffer(this->moonVertexesBuffer);
		Main::getSkyboxShader().colorsLocation->setVertexBuffer(this->moonColorsBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Skybox::initBuffers()
	{
		GLfloat moonTexCoords[] = {0, 0, 0, .5, .25, 0, .25, .5, .25, 0, 0, .5};
		GLfloat sunTexCoords[] = {0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1};
		GLfloat moonVertexes[] = {-100, 100, -500, -100, -100, -500, 100, 100, -500, 100, -100, -500, 100, 100, -500, -100, -100, -500};
		GLfloat sunVertexes[] = {-100, -100, 500, -100, 100, 500, 100, -100, 500, 100, 100, 500, 100, -100, 500, -100, 100, 500};
		GLfloat moonColors[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
		GLfloat sunColors[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
		this->moonTexCoordsBuffer.setData(GL_ARRAY_BUFFER, moonTexCoords, sizeof(moonTexCoords), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->sunTexCoordsBuffer.setData(GL_ARRAY_BUFFER, sunTexCoords, sizeof(sunTexCoords), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->moonVertexesBuffer.setData(GL_ARRAY_BUFFER, moonVertexes, sizeof(moonVertexes), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->sunVertexesBuffer.setData(GL_ARRAY_BUFFER, sunVertexes, sizeof(sunVertexes), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->moonColorsBuffer.setData(GL_ARRAY_BUFFER, moonColors, sizeof(moonColors), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->sunColorsBuffer.setData(GL_ARRAY_BUFFER, sunColors, sizeof(sunColors), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
	}

}
