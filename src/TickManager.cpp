#include "TickManager.h"
#include "Debug.h"
#include <algorithm>

extern int64_t nanotime;

namespace voxel
{

	int64_t TickManager::lastUpdate;
	int64_t TickManager::ticksToDo;
	float TickManager::delta;

	void TickManager::init()
	{
		lastUpdate = nanotime;
		ticksToDo = 0;
	}

	void TickManager::update()
	{
		int64_t passedTime = nanotime - lastUpdate;
		int64_t ticks = passedTime / (1000000000 / 20);
		ticksToDo = std::min(10l, ticks);
		lastUpdate += ticks * (1000000000 / 20);
		delta = (nanotime - lastUpdate) / (float)(1000000000 / 20);
	}

}
