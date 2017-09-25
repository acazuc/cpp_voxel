#include "Blocks.h"
#include <cstring>

namespace voxel
{

	Block **Blocks::blocks;

	void Blocks::init()
	{
		blocks = new Block*[255];
		std::memset(blocks, 0, sizeof(*blocks) * 255);
		blocks[0] = (new Block(0))->setDefaultName("air")->setOpacity(1)->setTransparent(true)->setSolid(false)->setReplaceable(true)->setFocusable(false);
		blocks[1] = (new Block(1))->setDefaultName("stone")->setHardness(1.5)->setResistance(10)->setTex(1. / 16, 0);
		blocks[2] = (new Block(2))->setDefaultName("grass")->setHardness(.6)->setTexTop(0, 0)->setTexSide(3. / 16, 0)->setTexBot(2. / 16, 0);
		blocks[3] = (new Block(3))->setDefaultName("dirt")->setHardness(.5)->setTex(2. / 16, 0);
		blocks[4] = (new Block(4))->setDefaultName("cobblestone")->setHardness(2)->setTex(0, 1. / 16);
		blocks[5] = (new Block(5))->setDefaultName("planks")->setHardness(2)->setTex(4. / 16, 0);
		blocks[6] = (new Block(6))->setDefaultName("sapling")->setHardness(0)->setTex(15. / 16, 0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		blocks[7] = (new Block(7))->setDefaultName("bedrock")->setHardness(6000000)->setTex(1. / 16, 1. / 16);
		blocks[8] = (new Block(8))->setDefaultName("flowing_water")->setHardness(100)->setTex(15. / 16, 13. / 16)->setLayer(2)->setOpacity(1)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		blocks[9] = (new Block(9))->setDefaultName("water")->setHardness(100)->setTex(15. / 16, 13. / 16)->setLayer(2)->setOpacity(1)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		blocks[10] = (new Block(10))->setDefaultName("flowing_lava")->setHardness(100)->setTex(15. / 16, 15. / 16)->setLayer(2)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		blocks[11] = (new Block(11))->setDefaultName("lava")->setHardness(100)->setTex(15. / 16, 13. / 16)->setLayer(2)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		blocks[12] = (new Block(12))->setDefaultName("sand")->setHardness(.5)->setTex(2. / 16, 1. / 16);
		blocks[13] = (new Block(13))->setDefaultName("gravel")->setHardness(.6)->setTex(3. / 16, 1. / 16);
		blocks[14] = (new Block(14))->setDefaultName("gold_ore")->setHardness(3)->setTex(0, 2. / 16);
		blocks[15] = (new Block(15))->setDefaultName("iron_ore")->setHardness(3)->setTex(1. / 16, 2. / 16);
		blocks[16] = (new Block(16))->setDefaultName("coal_ore")->setHardness(3)->setTex(2. / 16, 2. / 16);
		blocks[17] = (new Block(17))->setDefaultName("tree")->setHardness(2)->setTex(5. / 16, 1. / 16)->setTexSide(4. / 16, 1. / 16);
		blocks[18] = (new Block(18))->setDefaultName("tree_leaves")->setHardness(.2)->setTex(4. / 16, 3. / 16)->setLayer(1)->setTransparent(true)->setRenderSameNeighbor(true)->setOpacity(1);
		blocks[19] = (new Block(19))->setDefaultName("sponge")->setHardness(.6)->setTex(0, 3. / 16);
		blocks[20] = (new Block(20))->setDefaultName("glass")->setHardness(0.3)->setTex(1. / 16, 3. / 16);
		blocks[21] = (new Block(21))->setDefaultName("lapis_ore")->setHardness(3)->setTex(0, 10. / 16);
		blocks[22] = (new Block(22))->setDefaultName("lapis_block")->setHardness(3)->setTex(0, 9. / 16);
		blocks[23] = (new Block(23))->setDefaultName("dispenser")->setHardness(3.5)->setTex(14. / 16, 3. / 16)->setTexSide(13. / 16, 2. / 16)->setTexFront(14. / 16, 2. / 16);
		blocks[24] = (new Block(24))->setDefaultName("sandstone")->setHardness(.8)->setTexTop(0, 11. / 16)->setTexSide(0, 12. / 16)->setTexBot(0, 13. / 16);
		blocks[25] = (new Block(25))->setDefaultName("noteblock")->setHardness(.8)->setTex(10. / 16, 3. / 16)->setTexTop(11. / 16, 3. / 16);
		//
		blocks[37] = (new Block(37))->setDefaultName("yellow_flower")->setHardness(0)->setTex(13. / 16, 0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		blocks[38] = (new Block(38))->setDefaultName("red_flower")->setHardness(0)->setTex(12. / 16, 0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		blocks[39] = (new Block(39))->setDefaultName("brown_mushroom")->setHardness(0)->setTex(13. / 16, 1. / 16)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		blocks[40] = (new Block(40))->setDefaultName("red_mushroom")->setHardness(0)->setTex(12. / 16, 1. / 16)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		blocks[41] = (new Block(41))->setDefaultName("gold_block")->setHardness(3)->setTex(7. / 16, 1. / 16);
		blocks[42] = (new Block(42))->setDefaultName("iron_block")->setHardness(3)->setTex(6. / 16, 1. / 16);
		//
		blocks[45] = (new Block(45))->setDefaultName("bricks")->setHardness(2)->setTex(7. / 16, 0);
		blocks[46] = (new Block(46))->setDefaultName("tnt")->setHardness(0)->setTex(8. / 16, 0);
		blocks[47] = (new Block(47))->setDefaultName("bookshelf")->setHardness(1.5)->setTex(3. / 16, 2. / 16);
		blocks[48] = (new Block(48))->setDefaultName("mossy_cobblestone")->setHardness(2)->setTex(8. / 16, 2. / 16);
		blocks[49] = (new Block(49))->setDefaultName("obsidian")->setHardness(50)->setTex(9. / 16, 2. / 16);
		//
		blocks[52] = (new Block(52))->setDefaultName("mob_spawner")->setHardness(5)->setTex(1. / 16, 4. / 16);
		//
		blocks[56] = (new Block(56))->setDefaultName("diamond_ore")->setHardness(3)->setTex(2. / 16, 3. / 16);
		blocks[57] = (new Block(57))->setDefaultName("diamond_block")->setHardness(5)->setTex(8. / 16, 1. / 16);
		blocks[58] = (new Block(58))->setDefaultName("crafting_table")->setHardness(2.5)->setTexSide(11. / 16, 3. / 16)->setTexFront(12. / 16, 3. / 16)->setTexBack(12. / 16, 3. / 16)->setTexTop(11. / 16, 2. / 16)->setTexBot(4. / 16, 0);
	}

	void Blocks::clear()
	{
		for (uint8_t i = 0; i < 255; ++i)
			delete (blocks[i]);
		delete[] (blocks);
	}

}
