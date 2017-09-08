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
		blocks[4] = (new Block(5))->setDefaultName("planks")->setTex(4. / 16, 0);
		blocks[5] = (new Block(6))->setDefaultName("sapling")->setTex(15. / 16, 0);
		blocks[6] = (new Block(7))->setDefaultName("bedrock")->setTex(1. / 16, 1. / 16)->setUnbreakable(true);
		blocks[7] = (new Block(8))->setDefaultName("flowing_water")->setTex(15. / 16, 13. / 16);
		blocks[8] = (new Block(9))->setDefaultName("water")->setTex(15. / 16, 13. / 16);
		blocks[9] = (new Block(10))->setDefaultName("flowing_lava")->setTex(15. / 16, 15. / 16);
		blocks[10] = (new Block(11))->setDefaultName("lava")->setTex(15. / 16, 13. / 16);
		blocks[11] = (new Block(12))->setDefaultName("sand")->setTex(2. / 16, 1. / 16);
		blocks[12] = (new Block(13))->setDefaultName("gravel")->setTex(3. / 16, 1. / 16);
		blocks[13] = (new Block(14))->setDefaultName("gold_ore")->setTex(0, 2. / 16);
		blocks[14] = (new Block(15))->setDefaultName("iron_ore")->setTex(1. / 16, 2. / 16);
		blocks[15] = (new Block(16))->setDefaultName("coal_ore")->setTex(2. / 16, 2. / 16);
		blocks[16] = (new Block(17))->setDefaultName("tree")->setTex(5. / 16, 1. / 16)->setTexSide(4. / 16, 1. / 16);
		blocks[17] = (new Block(18))->setDefaultName("tree_leaves")->setTex(4. / 16, 3. / 16);
		blocks[18] = (new Block(19))->setDefaultName("sponge")->setTex(0, 3. / 16);
		blocks[19] = (new Block(20))->setDefaultName("glass")->setTex(1. / 16, 3. / 16);
		blocks[20] = (new Block(21))->setDefaultName("lapis_ore")->setTex(0, 10. / 16);
		blocks[21] = (new Block(22))->setDefaultName("lapis_block")->setTex(0, 9. / 16);
		blocks[22] = (new Block(23))->setDefaultName("dispenser")->setTex(14. / 16, 3. / 16)->setTexSide(13. / 16, 2. / 16)->setTexFront(14. / 16, 2. / 16);
		blocks[23] = (new Block(24))->setDefaultName("sandstone")->setTexTop(0, 11. / 16)->setTexSide(0, 12. / 16)->setTexBot(0, 13. / 16);
		blocks[24] = (new Block(25))->setDefaultName("noteblock")->setTex(10. / 16, 3. / 16)->setTexTop(11. / 16, 3. / 16);
	}

}
