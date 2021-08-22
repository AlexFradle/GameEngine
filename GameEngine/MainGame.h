#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <AGEngine/AGEngine.h>
#include <AGEngine/Sprite.h>
#include <AGEngine/GLSLProgram.h>
#include <AGEngine/GLTexture.h>
#include <AGEngine/Window.h>
#include <AGEngine/Camera2D.h>
#include <AGEngine/SpriteBatch.h>
#include <AGEngine/InputManager.h>
#include <AGEngine/Timing.h>
#include "Bullet.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	AGEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	AGEngine::GLSLProgram _colourProgram;
	AGEngine::Camera2D _camera;
	AGEngine::SpriteBatch _spriteBatch;
	AGEngine::InputManager _inputManager;
	AGEngine::FPSLimiter _fpsLimiter;
	std::vector<Bullet> _bullets;
	float _maxFps;
	float _fps;
	float _time;
	
};

