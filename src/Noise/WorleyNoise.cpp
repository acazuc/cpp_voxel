#include "WorleyNoise.h"
#include <librender/Vec/Vec2.h>
#include <algorithm>
#include <cmath>

#define K 0.142857142857f
#define Ko 0.428571428571f
#define JITTER 1.f

using librender::floor;
using librender::fract;
using librender::Vec2;
using librender::mod;
using librender::min;
using librender::max;

namespace voxel
{

	Vec3 WorleyNoise::permute(Vec3 x)
	{
		return (mod((x * 34.f + 1.f) * x, 10.f));
	}

	Vec3 WorleyNoise::dist(Vec3 x, Vec3 y)
	{
		return ((x * x + y * y));
	}

	double WorleyNoise::get2(double x, double y)
	{
		Vec2 P(x, y);
		Vec2 Pi = mod(floor(P), 10.f);
		Vec2 Pf = fract(P);
		//Vec3 oi = Vec3(-1, 0, 1);
		Vec3 oi = Vec3(-1, 0, 1);
		//Vec3 of = Vec3(-.5, .5, 1.5);
		Vec3 of = Vec3(-.5, .5, 1.5);
		Vec3 px = permute(oi + Pi.x);
		Vec3 p = permute(oi + px.x + Pi.y);
		Vec3 ox = fract(p * K) - Ko;
		Vec3 oy = mod(floor(p * K), 7.f) * K - Ko;
		Vec3 dx = Pf.x + .5f + ox * JITTER;
		Vec3 dy = Pf.y - of + oy * JITTER;
		Vec3 d1 = dist(dx, dy);
		p = permute(px.y + Pi.y + oi);
		ox = fract(p * K) - Ko;
		oy = mod(floor(p * K), 7.f) * K - Ko;
		dx = Pf.x - .5f + ox * JITTER;
		dy = Pf.y - of + oy * JITTER;
		Vec3 d2 = dist(dx, dy);
		p = permute(oi + px.z + Pi.y);
		ox = fract(p * K) - Ko;
		oy = mod(floor(p * K), 7.f) * K - Ko;
		dx = Pf.x - 1.5f + ox * JITTER;
		dy = Pf.y - of + oy * JITTER;
		Vec3 d3 = dist(dx,dy);
		Vec3 d1a = min(d1, d2);
		d2 = max(d1, d2);
		d2 = min(d2, d3);
		d1 = min(d1a, d2);
		d2 = max(d1a, d2);
		bool test = d1.x < d1.y;
		d1.x = test ? d1.x : d1.y;
		d1.y = test ? d1.y : d1.x;
		test = d1.x < d1.z;
		d1.x = test ? d1.x : d1.z;
		d1.z = test ? d1.z : d1.x;
		Vec2 tmp = min(Vec2(d1.y, d1.z), Vec2(d2.y, d2.z));
		d1.y = tmp.x;
		d1.z = tmp.y;
		d1.y = std::min(d1.y, d1.z);
		d1.y = std::min(d1.y, d2.x);
		return (sqrt(d1.x));
	}

}
