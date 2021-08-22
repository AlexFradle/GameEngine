#pragma once

namespace AGEngine {
	class FPSLimiter {
	public:
		FPSLimiter();
		~FPSLimiter();

		void init(float maxFPS);
		void begin();
		float end();
		void setmaxFPS(float maxFPS) { _maxFps = maxFPS; }

	private:
		void calculateFPS();

		float _frameTime;
		float _maxFps;
		float _fps;
		unsigned int _startTicks;
	};
}
