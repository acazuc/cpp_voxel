#include "GuiLagometer.h"
#include "Debug.h"
#include "Main.h"
#include "Gui.h"
#include <vector>

#define HEIGHT 166
#define PX_PER_MS 20

using librender::Vec3;
using librender::Vec2;

namespace voxel
{

	float GuiLagometer::values[512];
	uint32_t GuiLagometer::pos = 0;

	GuiLagometer::GuiLagometer()
	{
		for (uint32_t i = 0; i < 512; ++i)
			values[i] = 0;
	}

	void GuiLagometer::draw()
	{
		std::vector<Vec2> texCoords;
		std::vector<Vec2> vertexes;
		std::vector<Vec3> colors;
		for (uint32_t i = 0; i < 512; ++i)
		{
			int32_t diff = pos - i;
			if (diff < 0)
				diff += 512;
			float mix = std::min(1.f, values[i] / (1000 / 60));
			Vec3 color(mix, 1 - mix, 0);
			color *= (300 - diff) / 300.;
			colors.push_back(color);
			colors.push_back(color);
			vertexes.push_back(Vec2(i, 0));
			vertexes.push_back(Vec2(i, -values[i] * PX_PER_MS));
			texCoords.push_back(Vec2(0));
			texCoords.push_back(Vec2(0));
		}
		this->texCoordsBuffer.setData(GL_ARRAY_BUFFER, texCoords.data(), sizeof(Vec2) * texCoords.size(), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->vertexesBuffer.setData(GL_ARRAY_BUFFER, vertexes.data(), sizeof(Vec2) * vertexes.size(), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->colorsBuffer.setData(GL_ARRAY_BUFFER, colors.data(), sizeof(Vec3) * colors.size(), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		glBindTexture(GL_TEXTURE_2D, 0);
		Main::getTerrain()->bind();
		Main::getGuiShader().program->use();
		Main::getGuiShader().vertexesLocation->setVertexBuffer(this->vertexesBuffer);
		Main::getGuiShader().colorsLocation->setVertexBuffer(this->colorsBuffer);
		Mat4 model = Mat4::translate(Mat4(1), Vec3(0, Main::getWindow()->getHeight(), 0));
		Mat4 mvp = Gui::getMat() * model;
		Main::getGuiShader().mvpLocation->setMat4f(mvp);
		glLineWidth(1);
		glDrawArrays(GL_LINES, 0, colors.size());
	}

	void GuiLagometer::addValue(float val)
	{
		pos = (pos + 1) % 512;
		values[pos] = val;
	}

}
