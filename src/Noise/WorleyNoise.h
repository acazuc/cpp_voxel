#ifndef WORLEY_NOISE_H
# define WORLEY_NOISE_H

# include <glm/vec3.hpp>
# include <cstdint>

namespace voxel
{

	class WorleyNoise
	{

	private:
		static glm::vec3 permute(glm::vec3 x);
		static glm::vec3 dist(glm::vec3 x, glm::vec3 y);

	public:
		static double get2(double x, double y);

	};

}

#endif
