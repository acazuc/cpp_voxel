#ifndef BLOCK_H
# define BLOCK_H

# include <string>

namespace voxel
{

	class Block
	{

	private:
		std::string defaultName;
		uint8_t opacity;
		uint8_t light;
		uint8_t id;
		float resistance;
		float hardness;
		float texSideX;
		float texSideY;
		float texTopX;
		float texTopY;
		float texBotX;
		float texBotY;
		bool unbreakable;

	public:
		Block(uint8_t id);
		Block *setDefaultName(std::string defaultName);
		inline std::string &getDefaultName() {return (this->defaultName);};
		Block *setOpacity(uint8_t opacity);
		inline uint8_t getOpacity() {return (this->opacity);};
		Block *setLight(uint8_t light);
		inline uint8_t getLight() {return (this->light);};
		Block *setResistance(float resistance);
		inline float getResistance() {return (this->resistance);};
		Block *setHardness(float hardness);
		inline float getHardness() {return (this->hardness);};
		Block *setTexSide(float texSideX, float texSideY);
		Block *setTexSideX(float texSideX);
		inline float getTexSideX() {return (this->texSideX);};
		Block *setTexSideY(float texSideY);
		inline float getTexSideY() {return (this->texSideY);};
		Block *setTexTop(float texTopX, float texTopY);
		Block *setTexTopX(float texTopX);
		inline float getTexTopX() {return (this->texTopX);};
		Block *setTexTopY(float texTopY);
		inline float getTexTopY() {return (this->texTopY);};
		Block *setTexBot(float texBotX, float texBotY);
		Block *setTexBotX(float texBotX);
		inline float getTexBotX() {return (this->texBotX);};
		Block *setTexBotY(float texBotY);
		inline float getTexBotY() {return (this->texBotY);};
		Block *setTex(float texX, float texY);
		Block *setTexX(float texX);
		Block *setTexY(float texY);
		Block *setUnbreakable(bool unbreakable);
		inline bool isUnbreakable() {return (this->unbreakable);};

	};

}

#endif
