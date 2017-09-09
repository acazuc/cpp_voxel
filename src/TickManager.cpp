#include "TickManager.h"

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
		ticksToDo = passedTime / (1000000000 / 20);
		lastUpdate += ticksToDo * (1000000000 / 20);
		delta = (nanotime - lastUpdate) / (float)(1000000000 / 20);
	}

}
