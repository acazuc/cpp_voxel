#include "Level.h"
#include "NBT/NBTCompoundSanitizer.h"
#include "NBT/NBTMgr.h"
#include "Debug.h"

namespace voxel
{

	Level::Level(std::string path, std::string name)
	: path(path)
	, name(name)
	{
		this->level = new NBTGZipFile(path + "/level.dat");
		NBTTag *tag = NBTTag::readTag(this->level);
		if (tag && tag->getType() != NBT_TAG_COMPOUND)
			ERROR("Invalid level.dat");
		initNBT(reinterpret_cast<NBTTagCompound*>(tag));
	}

	Level::~Level()
	{
		delete (this->level);
	}

	void Level::initNBT(NBTTagCompound *NBT)
	{
		std::memset(&this->NBT, 0, sizeof(this->NBT));
		this->NBT.NBT = NBT;
		if (!this->NBT.NBT)
			this->NBT.NBT = new NBTTagCompound("");
		NBTCompoundSanitizer sanitizer(this->NBT.NBT);
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("Data", &this->NBT.Data));
		sanitizer.sanitize();
		NBTMgr::childTagCompoundDefault(this->NBT.NBT, &this->NBT.Data, "Data");
		sanitizer = NBTCompoundSanitizer(this->NBT.Data);
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("CustomBossEvents", &this->NBT.CustomBossEvents));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("DataPacks", &this->NBT.DataPacks));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("DimensionData", &this->NBT.DimensionData));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("version", &this->NBT.version, 19133));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("intitialized", &this->NBT.initialized, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryString("LevelName", &this->NBT.LevelName, this->name));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryString("generatorName", &this->NBT.generatorName, "default"));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("generatorVersion", &this->NBT.generatorVersion, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryString("generatorOptions", &this->NBT.generatorOptions, ""));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryLong("RandomSeed", &this->NBT.RandomSeed, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("MapFeatures", &this->NBT.MapFeatures, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryLong("LastPlayed", &this->NBT.LastPlayed, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("allowCommands", &this->NBT.allowCommands, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("hardcore", &this->NBT.hardcore, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("GameType", &this->NBT.GameType, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("Difficulty", &this->NBT.Difficulty, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("DifficultyLocked", &this->NBT.DifficultyLocked, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryLong("Time", &this->NBT.Time, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryLong("DayTime", &this->NBT.DayTime, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("SpawnX", &this->NBT.SpawnX, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("SpawnY", &this->NBT.SpawnY, 128));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("SpawnZ", &this->NBT.SpawnZ, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderCenterX", &this->NBT.BorderCenterX, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderCenterY", &this->NBT.BorderCenterY, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderCenterZ", &this->NBT.BorderCenterZ, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderSize", &this->NBT.BorderSize, 60000000));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderSafeZone", &this->NBT.BorderSafeZone, 5));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderWarningBlocks", &this->NBT.BorderWarningBlocks, 5));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderWarningTime", &this->NBT.BorderWarningTime, 15));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderSizeLerpTarget", &this->NBT.BorderSizeLerpTarget, 60000000));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryLong("BorderSizeLerpTime", &this->NBT.BorderSizeLerpTime, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryDouble("BorderDamagePerBlock", &this->NBT.BorderDamagePerBlock, 0.2));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("raining", &this->NBT.raining, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("rainTime", &this->NBT.rainTime, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryByte("thundering", &this->NBT.thundering, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("thunderTime", &this->NBT.thunderTime, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryInt("clearWeatherTime", &this->NBT.clearWeatherTime, 0));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("Player", &this->NBT.Player));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("GameRules", &this->NBT.GameRules));
		sanitizer.addEntry(new NBTCompoundSanitizerEntryCompound("Version", &this->NBT.Version));
		sanitizer.sanitize();
	}

}
