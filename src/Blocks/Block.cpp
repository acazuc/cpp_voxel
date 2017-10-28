#include "Block.h"

namespace voxel
{

	Block::Block(uint8_t id)
	: aabb(glm::vec3(0), glm::vec3(1))
	, opacity(15)
	, light(0)
	, layer(0)
	, id(id)
	, resistance(0)
	, hardness(0)
	, renderSameNeighbor(false)
	, transparent(false)
	, replaceable(false)
	, focusable(true)
	, fullCube(true)
	, solid(true)
	{
		//Empty
	}

	Block *Block::setDefaultName(std::string defaultName)
	{
		this->defaultName = defaultName;
		return (this);
	}

	Block *Block::setTool(enum ToolType tool)
	{
		this->tool = tool;
		return (this);
	}

	Block *Block::setAABB(AABB aabb)
	{
		this->aabb = aabb;
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

	Block *Block::setLayer(uint8_t layer)
	{
		this->layer = layer;
		return (this);
	}

	Block *Block::setRenderSameNeighbor(bool renderSameNeighbor)
	{
		this->renderSameNeighbor = renderSameNeighbor;
		return (this);
	}

	Block *Block::setTransparent(bool transparent)
	{
		this->transparent = transparent;
		return (this);
	}

	Block *Block::setReplaceable(bool replaceable)
	{
		this->replaceable = replaceable;
		return (this);
	}

	Block *Block::setFocusable(bool focusable)
	{
		this->focusable = focusable;
		return (this);
	}

	Block *Block::setFullCube(bool fullCube)
	{
		this->fullCube = fullCube;
		return (this);
	}

	Block *Block::setSolid(bool solid)
	{
		this->solid = solid;
		return (this);
	}

}
