#include "Blocks.h"
#include "./BlockRedstone.h"
#include "./BlockSapling.h"
#include "./BlockBase.h"
#include "./BlockRail.h"
#include <cstring>

namespace voxel
{

	Block **Blocks::blocks;

	void Blocks::init()
	{
		blocks = new Block*[255];
		std::memset(blocks, 0, sizeof(*blocks) * 255);
		blocks[0] = (new BlockBase(0))->setDefaultName("air")->setOpacity(1)->setTransparent(true)->setSolid(false)->setReplaceable(true)->setFocusable(false);
		blocks[1] = (new BlockBase(1))->setDefaultName("stone")->setHardness(1.5)->setResistance(10);
		reinterpret_cast<BlockBase*>(blocks[1])->setTex(1. / 16, 0);
		blocks[2] = (new BlockBase(2))->setDefaultName("grass")->setHardness(.6);
		reinterpret_cast<BlockBase*>(blocks[2])->setTexTop(0, 0)->setTexSide(3. / 16, 0)->setTexBot(2. / 16, 0);
		blocks[3] = (new BlockBase(3))->setDefaultName("dirt")->setHardness(.5);
		reinterpret_cast<BlockBase*>(blocks[3])->setTex(2. / 16, 0);
		blocks[4] = (new BlockBase(4))->setDefaultName("cobblestone")->setHardness(2);
		reinterpret_cast<BlockBase*>(blocks[4])->setTex(0, 1. / 16);
		blocks[5] = (new BlockBase(5))->setDefaultName("planks")->setHardness(2);
		reinterpret_cast<BlockBase*>(blocks[5])->setTex(4. / 16, 0);
		blocks[6] = (new BlockSapling(6))->setDefaultName("sapling")->setHardness(0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		reinterpret_cast<BlockSapling*>(blocks[6])->setTex(15. / 16, 0);
		blocks[7] = (new BlockBase(7))->setDefaultName("bedrock")->setHardness(6000000);
		reinterpret_cast<BlockBase*>(blocks[7])->setTex(1. / 16, 1. / 16);
		blocks[8] = (new BlockBase(8))->setDefaultName("flowing_water")->setHardness(100)->setLayer(2)->setOpacity(1)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		reinterpret_cast<BlockBase*>(blocks[8])->setTex(15. / 16, 13. / 16);
		blocks[9] = (new BlockBase(9))->setDefaultName("water")->setHardness(100)->setLayer(2)->setOpacity(1)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		reinterpret_cast<BlockBase*>(blocks[9])->setTex(15. / 16, 13. / 16);
		blocks[10] = (new BlockBase(10))->setDefaultName("flowing_lava")->setHardness(100)->setLayer(2)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		reinterpret_cast<BlockBase*>(blocks[10])->setTex(15. / 16, 15. / 16);
		blocks[11] = (new BlockBase(11))->setDefaultName("lava")->setHardness(100)->setLayer(2)->setTransparent(true)->setFocusable(false)->setSolid(false)->setReplaceable(true);
		reinterpret_cast<BlockBase*>(blocks[11])->setTex(15. / 16, 13. / 16);
		blocks[12] = (new BlockBase(12))->setDefaultName("sand")->setHardness(.5);
		reinterpret_cast<BlockBase*>(blocks[12])->setTex(2. / 16, 1. / 16);
		blocks[13] = (new BlockBase(13))->setDefaultName("gravel")->setHardness(.6);
		reinterpret_cast<BlockBase*>(blocks[13])->setTex(3. / 16, 1. / 16);
		blocks[14] = (new BlockBase(14))->setDefaultName("gold_ore")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[14])->setTex(0, 2. / 16);
		blocks[15] = (new BlockBase(15))->setDefaultName("iron_ore")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[15])->setTex(1. / 16, 2. / 16);
		blocks[16] = (new BlockBase(16))->setDefaultName("coal_ore")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[16])->setTex(2. / 16, 2. / 16);
		blocks[17] = (new BlockBase(17))->setDefaultName("tree")->setHardness(2);
		reinterpret_cast<BlockBase*>(blocks[17])->setTex(5. / 16, 1. / 16)->setTexSide(4. / 16, 1. / 16);
		blocks[18] = (new BlockBase(18))->setDefaultName("tree_leaves")->setHardness(.2)->setLayer(1)->setTransparent(true)->setRenderSameNeighbor(true)->setOpacity(1);
		reinterpret_cast<BlockBase*>(blocks[18])->setTex(4. / 16, 3. / 16);
		blocks[19] = (new BlockBase(19))->setDefaultName("sponge")->setHardness(.6);
		reinterpret_cast<BlockBase*>(blocks[19])->setTex(0, 3. / 16);
		blocks[20] = (new BlockBase(20))->setDefaultName("glass")->setHardness(0.3)->setTransparent(1)->setOpacity(0);
		reinterpret_cast<BlockBase*>(blocks[20])->setTex(1. / 16, 3. / 16);
		blocks[21] = (new BlockBase(21))->setDefaultName("lapis_ore")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[21])->setTex(0, 10. / 16);
		blocks[22] = (new BlockBase(22))->setDefaultName("lapis_block")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[22])->setTex(0, 9. / 16);
		blocks[23] = (new BlockBase(23))->setDefaultName("dispenser")->setHardness(3.5);
		reinterpret_cast<BlockBase*>(blocks[23])->setTex(14. / 16, 3. / 16)->setTexSide(13. / 16, 2. / 16)->setTexFront(14. / 16, 2. / 16);
		blocks[24] = (new BlockBase(24))->setDefaultName("sandstone")->setHardness(.8);
		reinterpret_cast<BlockBase*>(blocks[24])->setTexTop(0, 11. / 16)->setTexSide(0, 12. / 16)->setTexBot(0, 13. / 16);
		blocks[25] = (new BlockBase(25))->setDefaultName("noteblock")->setHardness(.8);
		reinterpret_cast<BlockBase*>(blocks[25])->setTex(10. / 16, 4. / 16);
		blocks[26] = (new BlockRail(26))->setDefaultName("golden_rail")->setHardness(.7)->setTransparent(true)->setOpacity(0);
		reinterpret_cast<BlockRail*>(blocks[26])->setTex(0, 8. / 16);
		blocks[27] = (new BlockRail(27))->setDefaultName("detector_rail")->setHardness(.7)->setTransparent(true)->setOpacity(0);
		reinterpret_cast<BlockRail*>(blocks[27])->setTex(0, 8. / 16);
		//
		blocks[37] = (new BlockSapling(37))->setDefaultName("yellow_flower")->setHardness(0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		reinterpret_cast<BlockSapling*>(blocks[37])->setTex(13. / 16, 0);
		blocks[38] = (new BlockSapling(38))->setDefaultName("red_flower")->setHardness(0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		reinterpret_cast<BlockSapling*>(blocks[38])->setTex(12. / 16, 0);
		blocks[39] = (new BlockSapling(39))->setDefaultName("brown_mushroom")->setHardness(0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		reinterpret_cast<BlockSapling*>(blocks[39])->setTex(13. / 16, 1. / 16);
		blocks[40] = (new BlockSapling(40))->setDefaultName("red_mushroom")->setHardness(0)->setFullCube(false)->setOpacity(0)->setSolid(false)->setLayer(1)->setTransparent(true)->setReplaceable(true);
		reinterpret_cast<BlockSapling*>(blocks[40])->setTex(12. / 16, 1. / 16);
		blocks[41] = (new BlockBase(41))->setDefaultName("gold_block")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[41])->setTex(7. / 16, 1. / 16);
		blocks[42] = (new BlockBase(42))->setDefaultName("iron_block")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[42])->setTex(6. / 16, 1. / 16);
		//
		blocks[45] = (new BlockBase(45))->setDefaultName("bricks")->setHardness(2);
		reinterpret_cast<BlockBase*>(blocks[45])->setTex(7. / 16, 0);
		blocks[46] = (new BlockBase(46))->setDefaultName("tnt")->setHardness(0);
		reinterpret_cast<BlockBase*>(blocks[46])->setTex(8. / 16, 0)->setTexTop(9. / 16, 0)->setTexBot(10. / 16, 0);
		blocks[47] = (new BlockBase(47))->setDefaultName("bookshelf")->setHardness(1.5);
		reinterpret_cast<BlockBase*>(blocks[47])->setTex(3. / 16, 2. / 16);
		blocks[48] = (new BlockBase(48))->setDefaultName("mossy_cobblestone")->setHardness(2);
		reinterpret_cast<BlockBase*>(blocks[48])->setTex(4. / 16, 2. / 16);
		blocks[49] = (new BlockBase(49))->setDefaultName("obsidian")->setHardness(50);
		reinterpret_cast<BlockBase*>(blocks[49])->setTex(5. / 16, 2. / 16);
		//
		blocks[52] = (new BlockBase(52))->setDefaultName("mob_spawner")->setHardness(5)->setTransparent(true)->setOpacity(1);
		reinterpret_cast<BlockBase*>(blocks[52])->setTex(1. / 16, 4. / 16);
		//
		blocks[54] = (new BlockBase(54))->setDefaultName("chest")->setHardness(2.5);
		reinterpret_cast<BlockBase*>(blocks[54])->setTex(10. / 16, 1. / 16)->setTexFront(11. / 16, 1. / 16)->setTexTop(9. / 16, 1. / 16)->setTexBot(9. / 16, 1. / 16);
		blocks[55] = (new BlockRedstone(55))->setDefaultName("redstone")->setHardness(0)->setTransparent(true)->setOpacity(0);
		reinterpret_cast<BlockRedstone*>(blocks[55])->setTex(4. / 16, 10. / 16);
		blocks[56] = (new BlockBase(56))->setDefaultName("diamond_ore")->setHardness(3);
		reinterpret_cast<BlockBase*>(blocks[56])->setTex(2. / 16, 3. / 16);
		blocks[57] = (new BlockBase(57))->setDefaultName("diamond_block")->setHardness(5);
		reinterpret_cast<BlockBase*>(blocks[57])->setTex(8. / 16, 1. / 16);
		blocks[58] = (new BlockBase(58))->setDefaultName("crafting_table")->setHardness(2.5);
		reinterpret_cast<BlockBase*>(blocks[58])->setTexSide(11. / 16, 3. / 16)->setTexFront(12. / 16, 3. / 16)->setTexBack(12. / 16, 3. / 16)->setTexTop(11. / 16, 2. / 16)->setTexBot(4. / 16, 0);
	}

	void Blocks::clear()
	{
		for (uint8_t i = 0; i < 255; ++i)
			delete (blocks[i]);
		delete[] (blocks);
	}

}
