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
		float texFrontX;
		float texFrontY;
		float texRightX;
		float texRightY;
		float texBackX;
		float texBackY;
		float texLeftX;
		float texLeftY;
		float texTopX;
		float texTopY;
		float texBotX;
		float texBotY;
		bool solid;

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
		Block *setTexSideY(float texSideY);
		Block *setTexFront(float texFrontX, float texFrontY);
		Block *setTexFrontX(float texFrontX);
		inline float getTexFrontX() {return (this->texFrontX);};
		Block *setTexFrontY(float texFrontY);
		inline float getTexFrontY() {return (this->texFrontY);};
		Block *setTexRight(float texRightX, float texRightY);
		Block *setTexRightX(float texRightX);
		inline float getTexRightX() {return (this->texRightX);};
		Block *setTexRightY(float texRightY);
		inline float getTexRightY() {return (this->texRightY);};
		Block *setTexBack(float texBackX, float texBackY);
		Block *setTexBackX(float texBackX);
		inline float getTexBackX() {return (this->texBackX);};
		Block *setTexBackY(float texBackY);
		inline float getTexBackY() {return (this->texBackY);};
		Block *setTexLeft(float texLeftX, float texLeftY);
		Block *setTexLeftX(float texLeftX);
		inline float getTexLeftX() {return (this->texLeftX);};
		Block *setTexLeftY(float texLeftY);
		inline float getTexLeftY() {return (this->texLeftY);};
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
		inline bool isSolid() {return (this->solid);};
		Block *setSolid(bool solid);

	};

}

#endif
