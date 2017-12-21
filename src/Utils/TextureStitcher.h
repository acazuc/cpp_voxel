#ifndef TEXTURE_STITCHER_H
# define TEXTURE_STITCHER_H

# include <librender/Texture.h>
# include <vector>

using librender::Texture;

namespace voxel
{

	typedef struct STextureStitcherTexture
	{
		uint8_t *data;
		int32_t width;
		int32_t height;
	} TextureStitcherTexture;

	class TextureStitcher
	{

	private:
		std::vector<TextureStitcherTexture> textures;
		Texture *texture;

	public:
		TextureStitcher();
		~TextureStitcher();
		void addTexture(uint8_t *data, int32_t width, int32_t height);
		void generate();

	};

}

#endif
