#include <iostream>
#include <Windows.h>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "MainGame.h"

// https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/

int main(int argc, char** argv) {
	//SDL_Init(SDL_INIT_EVERYTHING);
	MainGame mainGame;
	mainGame.run();
	return 0;
}