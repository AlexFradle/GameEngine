#pragma once
#include <vector>
#include <string>

namespace AGEngine {
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer);
	};
}
