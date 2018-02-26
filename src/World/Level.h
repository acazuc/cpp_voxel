#ifndef LEVEL_H
# define LEVEL_H

# include "NBT/NBTGZipFile.h"
# include "NBT/NBT.h"
# include "World.h"

namespace voxel
{

	struct LevelNBT
	{
		NBTTagCompound *NBT;
		NBTTagCompound *Data;
		NBTTagCompound *CustomBossEvents;
		NBTTagCompound *DataPacks;
		NBTTagCompound *DimensionData;
		NBTTagInt *version;
		NBTTagByte *initialized;
		NBTTagString *LevelName;
		NBTTagString *generatorName;
		NBTTagInt *generatorVersion;
		NBTTagString *generatorOptions;
		NBTTagLong *RandomSeed;
		NBTTagByte *MapFeatures;
		NBTTagLong *LastPlayed;
		NBTTagByte *allowCommands;
		NBTTagByte *hardcore;
		NBTTagInt *GameType;
		NBTTagByte *Difficulty;
		NBTTagByte *DifficultyLocked;
		NBTTagLong *Time;
		NBTTagLong *DayTime;
		NBTTagInt *SpawnX;
		NBTTagInt *SpawnY;
		NBTTagInt *SpawnZ;
		NBTTagDouble *BorderCenterX;
		NBTTagDouble *BorderCenterY;
		NBTTagDouble *BorderCenterZ;
		NBTTagDouble *BorderSize;
		NBTTagDouble *BorderSafeZone;
		NBTTagDouble *BorderWarningBlocks;
		NBTTagDouble *BorderWarningTime;
		NBTTagDouble *BorderSizeLerpTarget;
		NBTTagLong *BorderSizeLerpTime;
		NBTTagDouble *BorderDamagePerBlock;
		NBTTagByte *raining;
		NBTTagInt *rainTime;
		NBTTagByte *thundering;
		NBTTagInt *thunderTime;
		NBTTagInt *clearWeatherTime;
		NBTTagCompound *Player;
		NBTTagCompound *GameRules;
		NBTTagCompound *Version;
	};

	class Level
	{

	private:
		std::vector<World*> world;
		std::string path;
		std::string name;
		NBTGZipFile *level;
		FILE *lock;
		LevelNBT NBT;

	public:
		Level(std::string path, std::string name);
		~Level();
		void initNBT(NBTTagCompound *tag);

	};

}

#endif
