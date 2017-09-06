#include "Block.h"

namespace voxel
{

	Block::Block(uint8_t id)
	: id(id)
	{
		//Empty
	}

	Block *Block::setDefaultName(std::string defaultName)
	{
		this->defaultName = defaultName;
		return (this);
	}

	Block *Block::setOpacity(uint8_t opacity)
	{
		this->opacity = opacity;
		return (this);
	}

	Block *Block::setLight(uint8_t light)
	{
		this->light = light;
		return (this);
	}

	Block *Block::setResistance(float resistance)
	{
		this->resistance = resistance;
		return (this);
	}

	Block *Block::setHardness(float hardness)
	{
		this->hardness = hardness;
		return (this);
	}

	Block *Block::setTexSide(float texSideX, float texSideY)
	{
		setTexSideX(texSideX);
		setTexSideY(texSideY);
		return (this);
	}

	Block *Block::setTexSideX(float texSideX)
	{
		this->texSideX = texSideX;
		return (this);
	}

	Block *Block::setTexSideY(float texSideY)
	{
		this->texSideY = texSideY;
		return (this);
	}

	Block *Block::setTexTop(float texTopX, float texTopY)
	{
		setTexTopX(texTopX);
		setTexTopY(texTopY);
		return (this);
	}

	Block *Block::setTexTopX(float texTopX)
	{
		this->texTopX = texTopX;
		return (this);
	}

	Block *Block::setTexTopY(float texTopY)
	{
		this->texTopY = texTopY;
		return (this);
	}

	Block *Block::setTexBot(float texBotX, float texBotY)
	{
		setTexBotX(texBotX);
		setTexBotY(texBotY);
		return (this);
	}

	Block *Block::setTexBotX(float texBotX)
	{
		this->texBotX = texBotX;
		return (this);
	}

	Block *Block::setTexBotY(float texBotY)
	{
		this->texBotY = texBotY;
		return (this);
	}

	Block *Block::setTex(float texX, float texY)
	{
		setTexX(texX);
		setTexY(texY);
		return (this);
	}

	Block *Block::setTexX(float texX)
	{
		setTexSideX(texX);
		setTexTopX(texX);
		setTexBotX(texX);
		return (this);
	}

	Block *Block::setTexY(float texY)
	{
		setTexSideY(texY);
		setTexTopY(texY);
		setTexBotY(texY);
		return (this);
	}

	Block *Block::setUnbreakable(bool unbreakable)
	{
		this->unbreakable = unbreakable;
		return (this);
	}

}
