#include "Block.h"

namespace voxel
{

	Block::Block(uint8_t id)
	: opacity(15)
	, light(0)
	, layer(0)
	, id(id)
	, resistance(0)
	, hardness(0)
	, focusable(true)
	, solid(true)
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
		setTexFrontX(texSideX);
		setTexRightX(texSideX);
		setTexBackX(texSideX);
		setTexLeftX(texSideX);
		return (this);
	}

	Block *Block::setTexSideY(float texSideY)
	{
		setTexFrontY(texSideY);
		setTexRightY(texSideY);
		setTexBackY(texSideY);
		setTexLeftY(texSideY);
		return (this);
	}

	Block *Block::setTexFront(float texFrontX, float texFrontY)
	{
		setTexFrontX(texFrontX);
		setTexFrontY(texFrontY);
		return (this);
	}

	Block *Block::setTexFrontX(float texFrontX)
	{
		this->texFrontX = texFrontX;
		return (this);
	}

	Block *Block::setTexFrontY(float texFrontY)
	{
		this->texFrontY = texFrontY;
		return (this);
	}

	Block *Block::setTexRight(float texRightX, float texRightY)
	{
		setTexRightX(texRightX);
		setTexRightY(texRightY);
		return (this);
	}

	Block *Block::setTexRightX(float texRightX)
	{
		this->texRightX = texRightX;
		return (this);
	}

	Block *Block::setTexRightY(float texRightY)
	{
		this->texRightY = texRightY;
		return (this);
	}

	Block *Block::setTexBack(float texBackX, float texBackY)
	{
		setTexBackX(texBackX);
		setTexBackY(texBackY);
		return (this);
	}

	Block *Block::setTexBackX(float texBackX)
	{
		this->texBackX = texBackX;
		return (this);
	}

	Block *Block::setTexBackY(float texBackY)
	{
		this->texBackY = texBackY;
		return (this);
	}

	Block *Block::setTexLeft(float texLeftX, float texLeftY)
	{
		setTexLeftX(texLeftX);
		setTexLeftY(texLeftY);
		return (this);
	}

	Block *Block::setTexLeftX(float texLeftX)
	{
		this->texLeftX = texLeftX;
		return (this);
	}

	Block *Block::setTexLeftY(float texLeftY)
	{
		this->texLeftY = texLeftY;
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

	Block *Block::setLayer(uint8_t layer)
	{
		this->layer = layer;
		return (this);
	}

	Block *Block::setFocusable(bool focusable)
	{
		this->focusable = focusable;
		return (this);
	}

	Block *Block::setSolid(bool solid)
	{
		this->solid = solid;
		return (this);
	}

}
