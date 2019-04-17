#include "System.h"
#include <librender/GL.h>
#include <chrono>

namespace voxel
{

	int64_t System::nanotime()
	{
		return glfwGetTime() * 1000000000;
	}

	int64_t System::microtime()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
	}

}
