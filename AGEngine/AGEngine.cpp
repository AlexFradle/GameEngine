#include <SDL/SDL.h>
#include <GL/glew.h>
#include "AGEngine.h"

namespace AGEngine {
	int init() {
		// Initialise SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}