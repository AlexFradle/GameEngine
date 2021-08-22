#pragma once
#include "GLTexture.h"
#include <string>

namespace AGEngine {
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}
