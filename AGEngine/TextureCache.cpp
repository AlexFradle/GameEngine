#include "TextureCache.h"
#include "ImageLoader.h"

namespace AGEngine {
	TextureCache::TextureCache() {

	}

	TextureCache::~TextureCache() {

	}

	GLTexture TextureCache::getTexture(std::string texturePath) {
		auto mapIterator = _textureMap.find(texturePath);

		// if the texture is not in the map, insert it
		if (mapIterator == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
			_textureMap.insert(make_pair(texturePath, newTexture));

			return newTexture;
		}
		// otherwise return the texture of the existing one
		return mapIterator->second;
	}
}