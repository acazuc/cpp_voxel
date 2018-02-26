#ifndef NBT_COMPOUND_SANITIZER
# define NBT_COMPOUND_SANITIZER

# include "NBT/NBT.h"
# include <unordered_map>

namespace voxel
{

	struct NBTCompoundSanitizerEntry
	{
		enum NBTTagType type;
		std::string name;
		NBTTag **entry;
		NBTCompoundSanitizerEntry(enum NBTTagType type, std::string name, NBTTag **entry) : type(type), name(name), entry(entry) {};
	};

	class NBTCompoundSanitizer
	{

	private:
		std::unordered_map<std::string, NBTCompoundSanitizerEntry> entries;
		NBTTagCompound *tag;

	public:
		NBTCompoundSanitizer(NBTTagCompound *tag);
		void sanitize();
		inline void addEntry(NBTCompoundSanitizerEntry entry) {this->entries.emplace(entry.name, entry);};

	};

}

#endif
