#include "WorleyNoise.h"
#include <glm/vec2.hpp>
#include <glm/detail/func_common.hpp>

#define K 0.142857142857f
#define Ko 0.428571428571f
#define JITTER 1.f

namespace voxel
{

	glm::vec3 WorleyNoise::permute(glm::vec3 x)
	{
		return (glm::mod((x * 34.f + 1.f) * x, 10.f));
	}

	glm::vec3 WorleyNoise::dist(glm::vec3 x, glm::vec3 y)
	{
		return ((x * x + y * y));
	}

	double WorleyNoise::get2(double x, double y)
	{
		glm::vec2 P(x, y);
		glm::vec2 Pi = glm::mod(glm::floor(P), 10.f);
		glm::vec2 Pf = glm::fract(P);
		//glm::vec3 oi = glm::vec3(-1, 0, 1);
		glm::vec3 oi = glm::vec3(-1, 0, 1);
		//glm::vec3 of = glm::vec3(-.5, .5, 1.5);
		glm::vec3 of = glm::vec3(-.5, .5, 1.5);
		glm::vec3 px = permute(Pi.x + oi);
		glm::vec3 p = permute(px.x + Pi.y + oi);
		glm::vec3 ox = glm::fract(p * K) - Ko;
		glm::vec3 oy = glm::mod(glm::floor(p * K), 7.f) * K - Ko;
		glm::vec3 dx = Pf.x + .5f + JITTER * ox;
		glm::vec3 dy = Pf.y - of + JITTER * oy;
		glm::vec3 d1 = dist(dx, dy);
		p = permute(px.y + Pi.y + oi);
		ox = glm::fract(p * K) - Ko;
		oy = mod(floor(p * K), 7.f)*K - Ko;
		dx = Pf.x - .5f + JITTER * ox;
		dy = Pf.y - of + JITTER * oy;
		glm::vec3 d2 = dist(dx, dy);
		p = permute(px.z + Pi.y + oi);
		ox = glm::fract(p * K) - Ko;
		oy = glm::mod(glm::floor(p * K), 7.f) * K - Ko;
		dx = Pf.x - 1.5f + JITTER * ox;
		dy = Pf.y - of + JITTER * oy;
		glm::vec3 d3 = dist(dx,dy);
		glm::vec3 d1a = glm::min(d1, d2);
		d2 = glm::max(d1, d2);
		d2 = glm::min(d2, d3);
		d1 = glm::min(d1a, d2);
		d2 = glm::max(d1a, d2);
		bool test = d1.x < d1.y;
		d1.x = test ? d1.x : d1.y;
		d1.y = test ? d1.y : d1.x;
		test = d1.x < d1.z;
		d1.x = test ? d1.x : d1.z;
		d1.z = test ? d1.z : d1.x;
		glm::vec2 tmp = glm::min(glm::vec2(d1.y, d1.z), glm::vec2(d2.y, d2.z));
		d1.y = tmp.x;
		d1.z = tmp.y;
		d1.y = glm::min(d1.y, d1.z);
		d1.y = glm::min(d1.y, d2.x);
		return (sqrt(d1.x));
	}

}
