#include "Blocks.h"
#include <cstring>

namespace voxel
{

	Block **Blocks::blocks;

	void Blocks::init()
	{
		blocks = new Block*[256];
		std::memset(blocks, 0, sizeof(*blocks) * 256);
		blocks[0] = (new Block(0))->setDefaultName("air")->setOpacity(0);
		blocks[1] = (new Block(1))->setDefaultName("stone")->setHardness(1.5)->setResistance(10)->setTex(1. / 16, 0);
		blocks[2] = (new Block(2))->setDefaultName("grass")->setHardness(.6)->setTexTop(0, 0)->setTexSide(3. / 16, 0)->setTexBot(2. / 16, 0);
		blocks[3] = (new Block(3))->setDefaultName("dirt")->setHardness(.5)->setTex(2. / 16, 0);
		blocks[4] = (new Block(4))->setDefaultName("cobblestone")->setHardness(2)->setTex(0, 1. / 16);
		blocks[5] = (new Block(5))->setDefaultName("planks")->setHardness(2)->setTex(4. / 16, 0);
		blocks[6] = (new Block(6))->setDefaultName("sapling")->setHardness(0)->setTex(15. / 16, 0)->setSolid(false)->setLayer(1)->setOpacity(1);
		blocks[7] = (new Block(7))->setDefaultName("bedrock")->setHardness(6000000)->setTex(1. / 16, 1. / 16);
		blocks[8] = (new Block(8))->setDefaultName("flowing_water")->setHardness(100)->setTex(15. / 16, 13. / 16)->setLayer(2)->setOpacity(1)->setFocusable(false)->setSolid(false);
		blocks[9] = (new Block(9))->setDefaultName("water")->setHardness(100)->setTex(15. / 16, 13. / 16)->setLayer(2)->setOpacity(1)->setFocusable(false)->setSolid(false);
		blocks[10] = (new Block(10))->setDefaultName("flowing_lava")->setHardness(100)->setTex(15. / 16, 15. / 16)->setLayer(2)->setFocusable(false)->setSolid(false);
		blocks[11] = (new Block(11))->setDefaultName("lava")->setHardness(100)->setTex(15. / 16, 13. / 16)->setLayer(2)->setFocusable(false)->setSolid(false);
		blocks[12] = (new Block(12))->setDefaultName("sand")->setHardness(.5)->setTex(2. / 16, 1. / 16);
		blocks[13] = (new Block(13))->setDefaultName("gravel")->setHardness(.6)->setTex(3. / 16, 1. / 16);
		blocks[14] = (new Block(14))->setDefaultName("gold_ore")->setHardness(3)->setTex(0, 2. / 16);
		blocks[15] = (new Block(15))->setDefaultName("iron_ore")->setHardness(3)->setTex(1. / 16, 2. / 16);
		blocks[16] = (new Block(16))->setDefaultName("coal_ore")->setHardness(3)->setTex(2. / 16, 2. / 16);
		blocks[17] = (new Block(17))->setDefaultName("tree")->setHardness(2)->setTex(5. / 16, 1. / 16)->setTexSide(4. / 16, 1. / 16);
		blocks[18] = (new Block(18))->setDefaultName("tree_leaves")->setHardness(.2)->setTex(4. / 16, 3. / 16);
		blocks[19] = (new Block(19))->setDefaultName("sponge")->setHardness(.6)->setTex(0, 3. / 16);
		blocks[20] = (new Block(20))->setDefaultName("glass")->setHardness(0.3)->setTex(1. / 16, 3. / 16);
		blocks[21] = (new Block(21))->setDefaultName("lapis_ore")->setHardness(3)->setTex(0, 10. / 16);
		blocks[22] = (new Block(22))->setDefaultName("lapis_block")->setHardness(3)->setTex(0, 9. / 16);
		blocks[23] = (new Block(23))->setDefaultName("dispenser")->setHardness(3.5)->setTex(14. / 16, 3. / 16)->setTexSide(13. / 16, 2. / 16)->setTexFront(14. / 16, 2. / 16);
		blocks[24] = (new Block(24))->setDefaultName("sandstone")->setHardness(.8)->setTexTop(0, 11. / 16)->setTexSide(0, 12. / 16)->setTexBot(0, 13. / 16);
		blocks[25] = (new Block(25))->setDefaultName("noteblock")->setHardness(.8)->setTex(10. / 16, 3. / 16)->setTexTop(11. / 16, 3. / 16);
	}

	void Blocks::clear()
	{
		for (uint32_t i = 0; i < 256; ++i)
			delete (blocks[i]);
		delete[] (blocks);
	}

}
