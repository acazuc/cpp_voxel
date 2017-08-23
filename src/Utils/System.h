#ifndef SYSTEM_H
# define SYSTEM_H

# include <librender/GL.h>
# include <cstdint>
# include <string>
# include <chrono>
# include <unistd.h>

namespace voxel
{

	class System
	{

		public:
			static int64_t nanotime()
			{
				return (glfwGetTime() * 1000000000);
			};
			static int64_t microtime()
			{
				return (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count());
			};

	};

}

#endif

