#include "ErrorHandling.h"
#include <iostream>
#include <SDL/SDL.h>
#include <cstdlib>

namespace AGEngine {
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter key";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(8008);
	}
}