#include "Skybox.h"
#include "World/World.h"
#include "Debug.h"
#include "Main.h"
#include <libformat/PNG.h>

#define SKYBOX_PARTS 10
#define SKYBOX_DIST 1000

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
		LOG("loading terrain/sun.png");
		if (!libformat::PNG::read("data/textures/terrain/sun.png", fakeData, width, height))
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
		LOG("loading terrain/moon.png");
		if (!libformat::PNG::read("data/textures/terrain/moon.png", fakeData, width, height))
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
		updateSkyboxColors();
		Main::getSkyboxShader().program->use();
		Mat4 model(Mat4::translate(Mat4(1), Vec3(0, 0, 0)));
		model = Mat4::translate(model, this->world.getPlayer().getPos());
		Mat4 mvp(this->world.getPlayer().getViewProjMat() * model);
		Main::getSkyboxShader().mvpLocation->setMat4f(mvp);
		Main::getSkyboxShader().vertexesLocation->setVertexBuffer(this->skyboxVertexesBuffer);
		Main::getSkyboxShader().colorsLocation->setVertexBuffer(this->skyboxColorsBuffer);
		this->skyboxIndicesBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, 6 * ((SKYBOX_PARTS * SKYBOX_PARTS - 1) - (SKYBOX_PARTS - 1)), GL_UNSIGNED_INT, (void*)0);
		Main::getSunMoonShader().program->use();
		model = Mat4::translate(Mat4(1), this->world.getPlayer().getPos());
		model = Mat4::rotateX(model, -nanotime / 1000000000. / 60 / 20 * M_PI * 2);
		model = Mat4::scale(model, Vec3(1.5, 1.5, 1));
		mvp = this->world.getPlayer().getViewProjMat() * model;
		sun->bind();
		Main::getSunMoonShader().mvpLocation->setMat4f(mvp);
		Main::getSunMoonShader().texCoordsLocation->setVertexBuffer(this->sunTexCoordsBuffer);
		Main::getSunMoonShader().vertexesLocation->setVertexBuffer(this->sunVertexesBuffer);
		Main::getSunMoonShader().colorsLocation->setVertexBuffer(this->sunColorsBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		moon->bind();
		Main::getSunMoonShader().texCoordsLocation->setVertexBuffer(this->moonTexCoordsBuffer);
		Main::getSunMoonShader().vertexesLocation->setVertexBuffer(this->moonVertexesBuffer);
		Main::getSunMoonShader().colorsLocation->setVertexBuffer(this->moonColorsBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Skybox::initBuffers()
	{
		GLfloat moonTexCoords[] = {0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1};
		GLfloat sunTexCoords[] = {0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1};
		GLfloat moonVertexes[] = {-100, 100, -800, -100, -100, -800, 100, 100, -800, 100, -100, -800, 100, 100, -800, -100, -100, -800};
		GLfloat sunVertexes[] = {-100, -100, 800, -100, 100, 800, 100, -100, 800, 100, 100, 800, 100, -100, 800, -100, 100, 800};
		GLfloat moonColors[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
		GLfloat sunColors[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
		this->moonTexCoordsBuffer.setData(GL_ARRAY_BUFFER, moonTexCoords, sizeof(moonTexCoords), GL_FLOAT, 2, GL_STATIC_DRAW);
		this->sunTexCoordsBuffer.setData(GL_ARRAY_BUFFER, sunTexCoords, sizeof(sunTexCoords), GL_FLOAT, 2, GL_STATIC_DRAW);
		this->moonVertexesBuffer.setData(GL_ARRAY_BUFFER, moonVertexes, sizeof(moonVertexes), GL_FLOAT, 3, GL_STATIC_DRAW);
		this->sunVertexesBuffer.setData(GL_ARRAY_BUFFER, sunVertexes, sizeof(sunVertexes), GL_FLOAT, 3, GL_STATIC_DRAW);
		this->moonColorsBuffer.setData(GL_ARRAY_BUFFER, moonColors, sizeof(moonColors), GL_FLOAT, 3, GL_STATIC_DRAW);
		this->sunColorsBuffer.setData(GL_ARRAY_BUFFER, sunColors, sizeof(sunColors), GL_FLOAT, 3, GL_STATIC_DRAW);
		Vec3 *skyboxVertexes = new Vec3[SKYBOX_PARTS * SKYBOX_PARTS - (SKYBOX_PARTS - 1) * 2];
		GLuint *skyboxIndices = new GLuint[6 * ((SKYBOX_PARTS * SKYBOX_PARTS - 1) - (SKYBOX_PARTS - 1))];
		GLuint pos = 0;
		for (uint32_t y = 0; y < SKYBOX_PARTS; ++y)
		{
			for (uint32_t x = 0; x < SKYBOX_PARTS; ++x)
			{
				if ((y == 0 && x == 1) || (y > 0 && y < SKYBOX_PARTS - 1) || (y == SKYBOX_PARTS - 1 && x == 0))
				{
					GLuint index = getSkyboxIndex(x, y);
					float fac = M_PI * 2 / (SKYBOX_PARTS);
					float yFac = std::sin(y * M_PI / (SKYBOX_PARTS - 1));
					skyboxVertexes[index].x = std::cos(x * fac) * SKYBOX_DIST * yFac;
					skyboxVertexes[index].y = -std::cos(y * fac / 2) * SKYBOX_DIST;
					skyboxVertexes[index].z = std::sin(x * fac) * SKYBOX_DIST * yFac;
				}
				if (y == SKYBOX_PARTS - 1)
					continue;
				GLuint p1 = getSkyboxIndex(x, y);
				GLuint p2 = getSkyboxIndex((x + 1) % SKYBOX_PARTS, y);
				GLuint p3 = getSkyboxIndex((x + 1) % SKYBOX_PARTS, (y + 1) % SKYBOX_PARTS);
				GLuint p4 = getSkyboxIndex(x, (y + 1) % SKYBOX_PARTS);
				skyboxIndices[pos++] = p1;
				skyboxIndices[pos++] = p2;
				skyboxIndices[pos++] = p4;
				skyboxIndices[pos++] = p3;
				skyboxIndices[pos++] = p4;
				skyboxIndices[pos++] = p2;
			}
		}
		this->skyboxVertexesBuffer.setData(GL_ARRAY_BUFFER, skyboxVertexes, sizeof(*skyboxVertexes) * (SKYBOX_PARTS * SKYBOX_PARTS - (SKYBOX_PARTS - 1) * 2), GL_FLOAT, 3, GL_STATIC_DRAW);
		this->skyboxIndicesBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, skyboxIndices, sizeof(*skyboxIndices) * 6 * ((SKYBOX_PARTS * SKYBOX_PARTS - 1) - (SKYBOX_PARTS - 1)), GL_UNSIGNED_INT, 1, GL_STATIC_DRAW);
		delete[] (skyboxVertexes);
		delete[] (skyboxIndices);
	}

	uint32_t Skybox::getSkyboxIndex(uint32_t x, uint32_t y)
	{
		if (y == 0)
			return (0);
		if (y == SKYBOX_PARTS - 1)
			return (1 + (SKYBOX_PARTS - 2) * SKYBOX_PARTS);
		return (1 + (y - 1) * SKYBOX_PARTS + x);
	}

	void Skybox::updateSkyboxColors()
	{
		Vec3 *skyboxColors = new Vec3[SKYBOX_PARTS * SKYBOX_PARTS - (SKYBOX_PARTS - 1) * 2];
		Vec3 col;
		for (uint32_t y = 0; y < SKYBOX_PARTS; ++y)
		{
			if (y <= SKYBOX_PARTS / 2)
				col = Vec3(.71, .82, 1);
			else
				col = Vec3(.51, .68, 1);
			for (uint32_t x = 0; x < SKYBOX_PARTS; ++x)
			{
				if ((y == 0 && x == 1) || (y > 0 && y < SKYBOX_PARTS - 1) || (y == SKYBOX_PARTS - 1 && x == 0))
				{
					GLuint index = getSkyboxIndex(x, y);
					skyboxColors[index] = col;
				}
			}
		}
		this->skyboxColorsBuffer.setData(GL_ARRAY_BUFFER, skyboxColors, sizeof(*skyboxColors) * (SKYBOX_PARTS * SKYBOX_PARTS - (SKYBOX_PARTS - 1) * 2), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		delete[] (skyboxColors);
	}

}
