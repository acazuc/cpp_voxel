#include "GuiLagometer.h"
#include "Debug.h"
#include "Main.h"
#include "Gui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define HEIGHT 166
#define PX_PER_MS 20

namespace voxel
{

	float GuiLagometer::values[512];
	uint32_t GuiLagometer::pos = 0;

	GuiLagometer::GuiLagometer()
	{
		//
	}

	void GuiLagometer::draw()
	{
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec2> vertexes;
		std::vector<glm::vec3> colors;
		for (uint32_t i = 0; i < 512; ++i)
		{
			int32_t diff = pos - i;
			if (diff < 0)
				diff += 512;
			colors.push_back(glm::vec3(0, (300 - diff) / 300., 0));
			colors.push_back(glm::vec3(0, (300 - diff) / 300., 0));
			vertexes.push_back(glm::vec2(i, 0));
			vertexes.push_back(glm::vec2(i, -values[i] * PX_PER_MS));
			texCoords.push_back(glm::vec2(0));
			texCoords.push_back(glm::vec2(0));
		}
		this->texCoordsBuffer.setData(GL_ARRAY_BUFFER, texCoords.data(), sizeof(glm::vec2) * texCoords.size(), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes.data(), sizeof(glm::vec2) * vertexes.size(), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->colorsBuffer.setData(GL_ARRAY_BUFFER, colors.data(), sizeof(glm::vec3) * colors.size(), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		glBindTexture(GL_TEXTURE_2D, 0);
		Main::getTerrain()->bind();
		Main::getGuiShader().program->use();
		Main::getGuiShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		Main::getGuiShader().colorsLocation->setVertexBuffer(this->colorsBuffer);
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0, Main::getWindow()->getHeight(), 0));
		glm::mat4 mvp = Gui::getMat() * model;
		Main::getGuiShader().mvpLocation->setMat4f(mvp);
		glLineWidth(1);
		glDisable(GL_MULTISAMPLE);
		glDrawArrays(GL_LINES, 0, colors.size());
		glEnable(GL_MULTISAMPLE);
	}

	void GuiLagometer::addValue(float val)
	{
		pos = (pos + 1) % 512;
		values[pos] = val;
	}

}
