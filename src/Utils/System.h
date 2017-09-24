#ifndef SYSTEM_H
# define SYSTEM_H

# include <cstdint>

namespace voxel
{

	class System
	{

		public:
			static int64_t nanotime();
			static int64_t microtime();

	};

}

#endif

