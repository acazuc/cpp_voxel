#include "Blocks.h"

namespace voxel
{

	Block **Blocks::blocks;

	void Blocks::init()
	{
		blocks = new Block*[256];
		blocks[0] = (new Block(1))->setDefaultName("stone")->setHardness(1.5)->setResistance(10)->setTex(1. / 16, 0);
		blocks[1] = (new Block(2))->setDefaultName("grass")->setTexTop(0, 0)->setTexSide(3. / 16, 0)->setTexBot(2. / 16, 0);
		blocks[2] = (new Block(3))->setDefaultName("dirt")->setTex(2. / 16, 0);
		blocks[3] = (new Block(4))->setDefaultName("cobblestone")->setTex(0, 1. / 16);
		blocks[6] = (new Block(7))->setDefaultName("bedrock")->setTex(1. / 16, 1. / 16)->setUnbreakable(true);
	}

}
