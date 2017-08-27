#include "readfile.h"
#include <fstream>

namespace voxel
{

	std::string readfile(std::string name)
	{
		std::ifstream ifs(name);
		return (std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()));
	}

}
