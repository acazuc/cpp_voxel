#ifndef NBT_COMPOUND_SANITIZER
# define NBT_COMPOUND_SANITIZER

# include "NBT/NBT.h"
# include <unordered_map>

namespace voxel
{

	class NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;
	friend class NBTCompoundSanitizerEntryByte;
	friend class NBTCompoundSanitizerEntryShort;
	friend class NBTCompoundSanitizerEntryInt;
	friend class NBTCompoundSanitizerEntryLong;
	friend class NBTCompoundSanitizerEntryFloat;
	friend class NBTCompoundSanitizerEntryDouble;
	friend class NBTCompoundSanitizerEntryByteArray;
	friend class NBTCompoundSanitizerEntryString;
	friend class NBTCompoundSanitizerEntryList;
	friend class NBTCompoundSanitizerEntryCompound;
	friend class NBTCompoundSanitizerEntryIntArray;
	friend class NBTCompoundSanitizerEntryLongArray;

	private:
		enum NBTTagType type;
		std::string name;
		NBTTag **entry;
		NBTCompoundSanitizerEntry(enum NBTTagType type, std::string name, NBTTag **entry) : type(type), name(name), entry(entry) {};
		virtual ~NBTCompoundSanitizerEntry() {};

	};

	class NBTCompoundSanitizerEntryByte : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		int8_t value;
		NBTCompoundSanitizerEntryByte(std::string name, NBTTagByte **entry, int8_t value) : NBTCompoundSanitizerEntry(NBT_TAG_BYTE, name, reinterpret_cast<NBTTag**>(entry)), value(value) {};

	};

	class NBTCompoundSanitizerEntryShort : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		int16_t value;
		NBTCompoundSanitizerEntryShort(std::string name, NBTTagShort **entry, int16_t value) : NBTCompoundSanitizerEntry(NBT_TAG_SHORT, name, reinterpret_cast<NBTTag**>(entry)), value(value) {};

	};

	class NBTCompoundSanitizerEntryInt : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		int32_t value;
		NBTCompoundSanitizerEntryInt(std::string name, NBTTagInt **entry, int32_t value) : NBTCompoundSanitizerEntry(NBT_TAG_INT, name, reinterpret_cast<NBTTag**>(entry)), value(value) {};

	};

	class NBTCompoundSanitizerEntryLong : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		int64_t value;
		NBTCompoundSanitizerEntryLong(std::string name, NBTTagLong **entry, int64_t value) : NBTCompoundSanitizerEntry(NBT_TAG_LONG, name, reinterpret_cast<NBTTag**>(entry)), value(value) {};

	};

	class NBTCompoundSanitizerEntryFloat : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		float value;
		NBTCompoundSanitizerEntryFloat(std::string name, NBTTagFloat **entry, float value) : NBTCompoundSanitizerEntry(NBT_TAG_FLOAT, name, reinterpret_cast<NBTTag**>(entry)), value(value) {};

	};

	class NBTCompoundSanitizerEntryDouble : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		double value;
		NBTCompoundSanitizerEntryDouble(std::string name, NBTTagDouble **entry, double value) : NBTCompoundSanitizerEntry(NBT_TAG_DOUBLE, name, reinterpret_cast<NBTTag**>(entry)), value(value) {};

	};

	class NBTCompoundSanitizerEntryByteArray : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		int32_t len;
		NBTCompoundSanitizerEntryByteArray(std::string name, NBTTagByteArray **entry, int32_t len) : NBTCompoundSanitizerEntry(NBT_TAG_BYTE_ARRAY, name, reinterpret_cast<NBTTag**>(entry)), len(len) {};

	};

	class NBTCompoundSanitizerEntryString : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		std::string value;
		NBTCompoundSanitizerEntryString(std::string name, NBTTagString **entry, std::string value) : NBTCompoundSanitizerEntry(NBT_TAG_STRING, name, reinterpret_cast<NBTTag**>(entry)), value(value) {};

	};

	class NBTCompoundSanitizerEntryList : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		enum NBTTagType type;
		NBTCompoundSanitizerEntryList(std::string name, NBTTagList **entry, enum NBTTagType type) : NBTCompoundSanitizerEntry(NBT_TAG_LIST, name, reinterpret_cast<NBTTag**>(entry)), type(type) {};

	};

	class NBTCompoundSanitizerEntryCompound : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		NBTCompoundSanitizerEntryCompound(std::string name, NBTTagCompound **entry) : NBTCompoundSanitizerEntry(NBT_TAG_COMPOUND, name, reinterpret_cast<NBTTag**>(entry)) {};

	};

	class NBTCompoundSanitizerEntryIntArray : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		int32_t len;
		NBTCompoundSanitizerEntryIntArray(std::string name, NBTTagIntArray **entry, int32_t len) : NBTCompoundSanitizerEntry(NBT_TAG_INT_ARRAY, name, reinterpret_cast<NBTTag**>(entry)), len(len) {};

	};

	class NBTCompoundSanitizerEntryLongArray : public NBTCompoundSanitizerEntry
	{

	friend class NBTCompoundSanitizer;

	public:
		int32_t len;
		NBTCompoundSanitizerEntryLongArray(std::string name, NBTTagLongArray **entry, int32_t len) : NBTCompoundSanitizerEntry(NBT_TAG_LONG_ARRAY, name, reinterpret_cast<NBTTag**>(entry)), len(len) {};

	};

	class NBTCompoundSanitizer
	{

	private:
		std::unordered_map<std::string, NBTCompoundSanitizerEntry*> entries;
		NBTTagCompound *tag;

	public:
		NBTCompoundSanitizer(NBTTagCompound *tag);
		~NBTCompoundSanitizer();
		void sanitize();
		inline void addEntry(NBTCompoundSanitizerEntry *entry) {this->entries.emplace(entry->name, entry);};

	};

}

#endif
