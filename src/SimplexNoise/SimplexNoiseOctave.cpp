#include "SimplexNoiseOctave.h"
#include <cstring>

#define NUMBEROFSWAPS 4000

#define F2 0.36602540378443864676//0.5 * (sqrt(3.0) - 1.0);
#define G2 0.21132486540518711775//(3.0 - sqrt(3.0)) / 6.0;
#define F3 0.33333333333333333333//1.0 / 3.0;
#define G3 0.16666666666666666667//1.0 / 6.0;

static uint8_t p_supply[] = {151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

namespace voxel
{

	static SimplexNoiseGrad grads[] = {{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0}, {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}
		, {1, 0, -1}, {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}};

	void SimplexNoiseOctave::init(uint32_t seed)
	{
		uint8_t *p = new uint8_t[sizeof(p_supply)];
		std::memcpy(p, p_supply, sizeof(p_supply));
		seed = seed * 6364136223846793005LL + 1442695040888963407LL;
		seed = seed * 6364136223846793005LL + 1442695040888963407LL;
		seed = seed * 6364136223846793005LL + 1442695040888963407LL;
		for (uint32_t i = 0; i < NUMBEROFSWAPS; ++i)
		{
			seed = seed * 6364136223846793005LL + 1442695040888963407LL;
			int r = (int)((seed + 31) % (256));
			if (r < 0)
				r += (i + 1);
			short temp = p[r];
			p[r] = p[i % 256];
			p[i % 256] = temp;
		}
		for (uint32_t i = 0; i < 512; ++i)
		{
			this->perm[i] = p[i & 255];
			this->permMod12[i] = (uint8_t)(this->perm[i] % 12);
		}
		delete[] (p);

	}

	double SimplexNoiseOctave::get2(double xin, double yin)
	{
		double n0;
		double n1;
		double n2;
		double s = (xin + yin) * F2;
		int32_t i = fastFloor(xin + s);
		int32_t j = fastFloor(yin + s);
		double t = (i + j) * G2;
		double X0 = i - t;
		double Y0 = j - t;
		double x0 = xin - X0;
		double y0 = yin - Y0;
		uint8_t i1;
		uint8_t j1;
		if (x0 > y0)
		{
			i1 = 1;
			j1 = 0;
		}
		else
		{
			i1 = 0;
			j1 = 1;
		}
		double x1 = x0 - i1 + G2;
		double y1 = y0 - j1 + G2;
		double x2 = x0 - 1 + 2 * G2;
		double y2 = y0 - 1 + 2 * G2;
		uint8_t ii = i & 0xff;
		uint8_t jj = j & 0xff;
		uint8_t gi0 = this->permMod12[ii + this->perm[jj]];
		uint8_t gi1 = this->permMod12[ii + i1 + this->perm[jj + j1]];
		uint8_t gi2 = this->permMod12[ii + 1 + this->perm[jj + 1]];
		double t0 = .5 - x0 * x0 - y0 * y0;
		if (t0 < 0)
		{
			n0 = 0;
		}
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * dot2(&grads[gi0], x0, y0);
		}
		double t1 = .5 - x1 * x1 - y1 * y1;
		if (t1 < 0)
		{
			n1 = 0;
		}
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * dot2(&grads[gi1], x1, y1);
		}
		double t2 = .5 - x2 * x2 - y2 * y2;
		if (t2 < 0)
		{
			n2 = 0;
		}
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot2(&grads[gi2], x2, y2);
		}
		return (70 * (n0 + n1 + n2));
	}


	double SimplexNoiseOctave::get3(double xin, double yin, double zin)
	{
		double n0;
		double n1;
		double n2;
		double n3;
		double s = (xin + yin + zin) * F3;
		int32_t i = fastFloor(xin + s);
		int32_t j = fastFloor(yin + s);
		int32_t k = fastFloor(zin + s);
		double t = (i + j + k) * G3;
		double X0 = i - t;
		double Y0 = j - t;
		double Z0 = k - t;
		double x0 = xin - X0;
		double y0 = yin - Y0;
		double z0 = zin - Z0;
		uint8_t i1;
		uint8_t j1;
		uint8_t k1;
		uint8_t i2;
		uint8_t j2;
		uint8_t k2;
		if (x0 >= y0)
		{
			if (y0 >= z0)
			{
				i1 = 1;
				j1 = 0;
				k1 = 0;
				i2 = 1;
				j2 = 1;
				k2 = 0;
			}
			else if (x0 >= z0)
			{
				i1 = 1;
				j1 = 0;
				k1 = 0;
				i2 = 1;
				j2 = 0;
				k2 = 1;
			}
			else
			{
				i1 = 0;
				j1 = 0;
				k1 = 1;
				i2 = 1;
				j2 = 0;
				k2 = 1;
			}
		}
		else
		{
			if (y0 < z0)
			{
				i1 = 0;
				j1 = 0;
				k1 = 1;
				i2 = 0;
				j2 = 1;
				k2 = 1;
			}
			else if (x0 < z0)
			{
				i1 = 0;
				j1 = 1;
				k1 = 0;
				i2 = 0;
				j2 = 1;
				k2 = 1;
			}
			else
			{
				i1 = 0;
				j1 = 1;
				k1 = 0;
				i2 = 1;
				j2 = 1;
				k2 = 0;
			}
		}
		double x1 = x0 - i1 + G3;
		double y1 = y0 - j1 + G3;
		double z1 = z0 - k1 + G3;
		double x2 = x0 - i2 + 2 * G3;
		double y2 = y0 - j2 + 2 * G3;
		double z2 = z0 - k2 + 2 * G3;
		double x3 = x0 - 1 + 3 * G3;
		double y3 = y0 - 1 + 3 * G3;
		double z3 = z0 - 1 + 3 * G3;
		uint8_t ii = i & 0xff;
		uint8_t jj = j & 0xff;
		uint8_t kk = k & 0xff;
		uint8_t gi0 = this->permMod12[ii + this->perm[jj + this->perm[kk]]];
		uint8_t gi1 = this->permMod12[ii + i1 + this->perm[jj + j1 + this->perm[kk + k1]]];
		uint8_t gi2 = this->permMod12[ii + i2 + this->perm[jj + j2 + this->perm[kk + k2]]];
		uint8_t gi3 = this->permMod12[ii + 1 + this->perm[jj + 1 + this->perm[kk + 1]]];
		double t0 = .6 - x0 * x0 - y0 * y0 - z0 * z0;
		if (t0 < 0)
		{
			n0 = 0;
		}
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * dot3(&grads[gi0], x0, y0, z0);
		}
		double t1 = 0.6 - x1 * x1 - y1 * y1 - z1 * z1;
		if (t1 < 0)
		{
			n1 = 0;
		}
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * dot3(&grads[gi1], x1, y1, z1);
		}
		double t2 = .6 - x2 * x2 - y2 * y2 - z2 * z2;
		if (t2 < 0)
		{
			n2 = 0;
		}
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot3(&grads[gi2], x2, y2, z2);
		}
		double t3 = .6 - x3 * x3 - y3 * y3 - z3 * z3;
		if (t3 < 0)
		{
			n3 = 0;
		}
		else
		{
			t3 *= t3;
			n3 = t3 * t3 * dot3(&grads[gi3], x3, y3, z3);
		}
		return (32 * (n0 + n1 + n2 + n3));
	}

	/*double SimplexNoiseOctave::get4(double x, double y, double z, double w)
	{
		double n0, n1, n2, n3, n4; // Noise contributions from the five corners
		// Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
		double s = (x + y + z + w) * F4; // Factor for 4D skewing
		int i = fastfloor(x + s);
		int j = fastfloor(y + s);
		int k = fastfloor(z + s);
		int l = fastfloor(w + s);
		double t = (i + j + k + l) * G4; // Factor for 4D unskewing
		double X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
		double Y0 = j - t;
		double Z0 = k - t;
		double W0 = l - t;
		double x0 = x - X0;  // The x,y,z,w distances from the cell origin
		double y0 = y - Y0;
		double z0 = z - Z0;
		double w0 = w - W0;
		// For the 4D case, the simplex is a 4D shape I won't even try to describe.
		// To find out which of the 24 possible simplices we're in, we need to
		// determine the magnitude ordering of x0, y0, z0 and w0.
		// Six pair-wise comparisons are performed between each possible pair
		// of the four coordinates, and the results are used to rank the numbers.
		int rankx = 0;
		int ranky = 0;
		int rankz = 0;
		int rankw = 0;
		if(x0 > y0) rankx++; else ranky++;
		if(x0 > z0) rankx++; else rankz++;
		if(x0 > w0) rankx++; else rankw++;
		if(y0 > z0) ranky++; else rankz++;
		if(y0 > w0) ranky++; else rankw++;
		if(z0 > w0) rankz++; else rankw++;
		int i1, j1, k1, l1; // The integer offsets for the second simplex corner
		int i2, j2, k2, l2; // The integer offsets for the third simplex corner
		int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner
		// simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
		// Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
		// impossible. Only the 24 indices which have non-zero entries make any sense.
		// We use a thresholding to set the coordinates in turn from the largest magnitude.
		// Rank 3 denotes the largest coordinate.
		i1 = rankx >= 3 ? 1 : 0;
		j1 = ranky >= 3 ? 1 : 0;
		k1 = rankz >= 3 ? 1 : 0;
		l1 = rankw >= 3 ? 1 : 0;
		// Rank 2 denotes the second largest coordinate.
		i2 = rankx >= 2 ? 1 : 0;
		j2 = ranky >= 2 ? 1 : 0;
		k2 = rankz >= 2 ? 1 : 0;
		l2 = rankw >= 2 ? 1 : 0;
		// Rank 1 denotes the second smallest coordinate.
		i3 = rankx >= 1 ? 1 : 0;
		j3 = ranky >= 1 ? 1 : 0;
		k3 = rankz >= 1 ? 1 : 0;
		l3 = rankw >= 1 ? 1 : 0;
		// The fifth corner has all coordinate offsets = 1, so no need to compute that.
		double x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
		double y1 = y0 - j1 + G4;
		double z1 = z0 - k1 + G4;
		double w1 = w0 - l1 + G4;
		double x2 = x0 - i2 + 2.0*G4; // Offsets for third corner in (x,y,z,w) coords
		double y2 = y0 - j2 + 2.0*G4;
		double z2 = z0 - k2 + 2.0*G4;
		double w2 = w0 - l2 + 2.0*G4;
		double x3 = x0 - i3 + 3.0*G4; // Offsets for fourth corner in (x,y,z,w) coords
		double y3 = y0 - j3 + 3.0*G4;
		double z3 = z0 - k3 + 3.0*G4;
		double w3 = w0 - l3 + 3.0*G4;
		double x4 = x0 - 1.0 + 4.0*G4; // Offsets for last corner in (x,y,z,w) coords
		double y4 = y0 - 1.0 + 4.0*G4;
		double z4 = z0 - 1.0 + 4.0*G4;
		double w4 = w0 - 1.0 + 4.0*G4;
		// Work out the hashed gradient indices of the five simplex corners
		int ii = i & 255;
		int jj = j & 255;
		int kk = k & 255;
		int ll = l & 255;
		int gi0 = perm[ii+perm[jj+perm[kk+perm[ll]]]] % 32;
		int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1+perm[ll+l1]]]] % 32;
		int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2+perm[ll+l2]]]] % 32;
		int gi3 = perm[ii+i3+perm[jj+j3+perm[kk+k3+perm[ll+l3]]]] % 32;
		int gi4 = perm[ii+1+perm[jj+1+perm[kk+1+perm[ll+1]]]] % 32;
		// Calculate the contribution from the five corners
		double t0 = 0.6 - x0*x0 - y0*y0 - z0*z0 - w0*w0;
		if(t0<0) n0 = 0.0;
		else {
	        	t0 *= t0;
	        	n0 = t0 * t0 * dot(grad4[gi0], x0, y0, z0, w0);
		}
		double t1 = 0.6 - x1*x1 - y1*y1 - z1*z1 - w1*w1;
		if(t1<0) n1 = 0.0;
		else {
	        	t1 *= t1;
	        	n1 = t1 * t1 * dot(grad4[gi1], x1, y1, z1, w1);
		}
		double t2 = 0.6 - x2*x2 - y2*y2 - z2*z2 - w2*w2;
		if(t2<0) n2 = 0.0;
		else {
	        	t2 *= t2;
	        	n2 = t2 * t2 * dot(grad4[gi2], x2, y2, z2, w2);
		}
		double t3 = 0.6 - x3*x3 - y3*y3 - z3*z3 - w3*w3;
		if(t3<0) n3 = 0.0;
		else {
	        	t3 *= t3;
	        	n3 = t3 * t3 * dot(grad4[gi3], x3, y3, z3, w3);
		}
		double t4 = 0.6 - x4*x4 - y4*y4 - z4*z4 - w4*w4;
		if(t4<0) n4 = 0.0;
		else {
	        	t4 *= t4;
	        	n4 = t4 * t4 * dot(grad4[gi4], x4, y4, z4, w4);
		}
		// Sum up and scale the result to cover the range [-1,1]
		return 27.0 * (n0 + n1 + n2 + n3 + n4);
	}*/

	double SimplexNoiseOctave::dot2(SimplexNoiseGrad *g, double x, double y)
	{
		return (g->x * x + g->y * y);
	}

	double SimplexNoiseOctave::dot3(SimplexNoiseGrad *g, double x, double y, double z)
	{
		return (g->x * x + g->y * y + g->z * z);
	}

	/*double SimplexNoiseOctave::dot4(SimplexNoiseGrad *g, double x, double y, double z, double w)
	{
		return (g->x * x + g->y * y + g->z * z + g->w * w);
	}*/

	int32_t SimplexNoiseOctave::fastFloor(double x)
	{
		int32_t xi = x;
		return (x < xi ? xi - 1 : xi);
	}

}
