#ifndef BLOCK_H
# define BLOCK_H

# include "Items/ToolType.h"
# include "AABB.h"
# include <string>

namespace voxel
{

	class ChunkTessellator;
	class Chunk;

	class Block
	{

	protected:
		std::string defaultName;
		enum ToolType tool;
		AABB aabb;
		uint8_t opacity;
		uint8_t light;
		uint8_t layer;
		uint8_t id;
		float resistance;
		float hardness;
		bool renderSameNeighbor;
		bool transparent;
		bool replaceable;
		bool focusable;
		bool fullCube;
		bool solid;

	public:
		Block(uint8_t id);
		virtual ~Block() {};
		virtual void draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tessellator, uint8_t visibleFaces, float *lights) {(void)chunk;(void)pos;(void)tessellator;(void)visibleFaces;(void)lights;};
		Block *setDefaultName(std::string defaultName);
		inline std::string &getDefaultName() {return (this->defaultName);};
		Block *setTool(enum ToolType tool);
		inline enum ToolType getTool() {return (this->tool);};
		Block *setAABB(AABB aabb);
		inline AABB &getAABB() {return (this->aabb);};
		Block *setOpacity(uint8_t opacity);
		inline uint8_t getOpacity() {return (this->opacity);};
		Block *setLight(uint8_t light);
		inline uint8_t getLight() {return (this->light);};
		inline virtual float getDestroyTexX() {return (0);};
		inline virtual float getDestroyTexY() {return (0);};
		Block *setResistance(float resistance);
		inline float getResistance() {return (this->resistance);};
		Block *setHardness(float hardness);
		inline float getHardness() {return (this->hardness);};
		Block *setLayer(uint8_t layer);
		inline uint8_t getLayer() {return (this->layer);};
		Block *setRenderSameNeighbor(bool renderSameNeighbor);
		inline bool isRenderSameNeighbor() {return (this->renderSameNeighbor);};
		Block *setTransparent(bool transparent);
		inline bool isTransparent() {return (this->transparent);};
		Block *setReplaceable(bool replaceable);
		inline bool isReplaceable() {return (this->replaceable);};
		Block *setFocusable(bool focusable);
		inline bool isFocusable() {return (this->focusable);};
		Block *setFullCube(bool fullCube);
		inline bool isFullCube() {return (this->fullCube);};
		Block *setSolid(bool solid);
		inline bool isSolid() {return (this->solid);};

	};

}

#endif
