#ifndef BLOCK_BASE_H
# define BLOCK_BASE_H

# include "./Block.h"

namespace voxel
{

	class BlockBase : public Block
	{

	private:
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

	public:
		BlockBase(uint8_t id);
		virtual void draw(Chunk *chunk, glm::vec3 pos, ChunkTessellator &tesselator, uint8_t visibleFaces, float *lights);
		virtual float getDestroyTexX();
		virtual float getDestroyTexY();
		BlockBase *setTexSide(float texSideX, float texSideY);
		BlockBase *setTexSideX(float texSideX);
		BlockBase *setTexSideY(float texSideY);
		BlockBase *setTexFront(float texFrontX, float texFrontY);
		BlockBase *setTexFrontX(float texFrontX);
		inline float getTexFrontX() {return (this->texFrontX);};
		BlockBase *setTexFrontY(float texFrontY);
		inline float getTexFrontY() {return (this->texFrontY);};
		BlockBase *setTexRight(float texRightX, float texRightY);
		BlockBase *setTexRightX(float texRightX);
		inline float getTexRightX() {return (this->texRightX);};
		BlockBase *setTexRightY(float texRightY);
		inline float getTexRightY() {return (this->texRightY);};
		BlockBase *setTexBack(float texBackX, float texBackY);
		BlockBase *setTexBackX(float texBackX);
		inline float getTexBackX() {return (this->texBackX);};
		BlockBase *setTexBackY(float texBackY);
		inline float getTexBackY() {return (this->texBackY);};
		BlockBase *setTexLeft(float texLeftX, float texLeftY);
		BlockBase *setTexLeftX(float texLeftX);
		inline float getTexLeftX() {return (this->texLeftX);};
		BlockBase *setTexLeftY(float texLeftY);
		inline float getTexLeftY() {return (this->texLeftY);};
		BlockBase *setTexTop(float texTopX, float texTopY);
		BlockBase *setTexTopX(float texTopX);
		inline float getTexTopX() {return (this->texTopX);};
		BlockBase *setTexTopY(float texTopY);
		inline float getTexTopY() {return (this->texTopY);};
		BlockBase *setTexBot(float texBotX, float texBotY);
		BlockBase *setTexBotX(float texBotX);
		inline float getTexBotX() {return (this->texBotX);};
		BlockBase *setTexBotY(float texBotY);
		inline float getTexBotY() {return (this->texBotY);};
		BlockBase *setTex(float texX, float texY);
		BlockBase *setTexX(float texX);
		BlockBase *setTexY(float texY);

	};

}

#endif
