#include "Window.h"
#include "ErrorHandling.h"

namespace AGEngine {
	Window::Window() {

	}

	Window::~Window() {

	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, Colour colour, unsigned int currentFlags) {
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;

		if (currentFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		if (currentFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL_GL contxt could not be created");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not init glew");
		}

		std::printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));

		glClearColor(
			colour.r > 0 ? (float)colour.r / 255 : 0.0f,
			colour.g > 0 ? (float)colour.g / 255 : 0.0f,
			colour.b > 0 ? (float)colour.b / 255 : 0.0f,
			colour.a > 0 ? (float)colour.a / 255 : 0.0f
		);

		SDL_GL_SetSwapInterval(0);  // uncap fps by disabiling vsync

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}
}