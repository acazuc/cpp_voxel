#ifndef TICK_MANAGER_H
# define TICK_MANAGER_H

# include <cstdint>

namespace voxel
{

	class TickManager
	{

	private:
		static int64_t lastUpdate;
		static int64_t ticksToDo;
		static float delta;

	public:
		static void init();
		static void update();
		static inline int64_t getTicksToDo() {return (ticksToDo);};
		static inline float getDelta() {return (delta);};

	};

}

#endif
