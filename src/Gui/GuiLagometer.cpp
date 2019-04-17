#include "GuiLagometer.h"
#include "Debug.h"
#include "Main.h"
#include "Gui.h"
#include <vector>

#define HEIGHT 166
#define PX_PER_MS 10

using librender::Vec3;
using librender::Vec2;

namespace voxel
{

	float GuiLagometer::values[512];
	uint32_t GuiLagometer::pos = 0;

	GuiLagometer::GuiLagometer()
	{
		for (size_t i = 0; i < 512; ++i)
			values[i] = 0;
		this->background.setTexture(Main::getEmpty());
		this->background.setProgram(Gui::getShaderSpriteProgram());
		this->background.setSize(512, 1000. / 60 * PX_PER_MS);
		this->background.setPos(0, Main::getWindow()->getHeight() - 1000. / 60 * PX_PER_MS);
		Color color(0, 0);
		this->background.setBotColor(color);
		color = Color(0, .5);
		this->background.setTopColor(color);
	}

	void GuiLagometer::draw()
	{
		this->background.draw(Gui::getMat());
		std::vector<Vec2> positions;
		std::vector<Vec3> colors;
		for (size_t i = 0; i < 512; ++i)
		{
			int32_t diff = pos - i;
			if (diff < 0)
				diff += 512;
			float mix = std::min(1.f, values[i] / (1000 / 60));
			Vec3 color(mix, 1 - mix, 0);
			color *= (300 - diff) / 300.;
			positions.push_back(Vec2(i, 0));
			positions.push_back(Vec2(i, -values[i] * PX_PER_MS));
			colors.push_back(color);
			colors.push_back(color);
		}
		this->positionBuffer.setData(GL_ARRAY_BUFFER, positions.data(), sizeof(*positions.data()) * positions.size(), GL_DYNAMIC_DRAW);
		this->colorBuffer.setData(GL_ARRAY_BUFFER, colors.data(), sizeof(*colors.data()) * colors.size(), GL_DYNAMIC_DRAW);
		glBindTexture(GL_TEXTURE_2D, 0);
		Main::getTerrain()->bind();
		Main::getGuiShader().program->use();
		Main::getGuiShader().vertexPositionLocation.setVertexBuffer(this->positionBuffer, 2, GL_FLOAT);
		Main::getGuiShader().vertexColorLocation.setVertexBuffer(this->colorBuffer, 3, GL_FLOAT);
		Mat4 model = Mat4::translate(Mat4(1), Vec3(0, Main::getWindow()->getHeight(), 0));
		Mat4 mvp = Gui::getMat() * model;
		Main::getGuiShader().mvpLocation.setMat4f(mvp);
		glLineWidth(1);
		glDrawArrays(GL_LINES, 0, colors.size());
	}

	void GuiLagometer::addValue(float val)
	{
		pos = (pos + 1) % 512;
		values[pos] = val;
	}

}
