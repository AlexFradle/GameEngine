#include "Timing.h"
#include <SDL/SDL.h>

namespace AGEngine {
	FPSLimiter::FPSLimiter() {

	}

	FPSLimiter::~FPSLimiter() {

	}

	void FPSLimiter::init(float maxFPS) {
		setmaxFPS(maxFPS);
	}

	void FPSLimiter::begin() {
		_startTicks = SDL_GetTicks();
	}

	float FPSLimiter::end() {
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		if (1000.0f / _maxFps > frameTicks)
			SDL_Delay(1000.0f / _maxFps - frameTicks);

		return _fps;
	}

	void FPSLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int curFrame = 0;
		static float prevTicks = SDL_GetTicks();
		float curTicks = SDL_GetTicks();

		_frameTime = curTicks - prevTicks;
		frameTimes[curFrame % NUM_SAMPLES] = _frameTime;
		prevTicks = curTicks;

		int count;
		curFrame++;
		if (curFrame < NUM_SAMPLES)
			count = curFrame;
		else
			count = NUM_SAMPLES;

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
			_fps = 1000.0f / frameTimeAverage;
		else
			_fps = 60.0f;
	}
}
