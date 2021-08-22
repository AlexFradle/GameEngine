#pragma once
#include <AGEngine/Window.h>
#include <AGEngine/GLSLProgram.h>
#include <AGEngine/Camera2D.h>
#include <AGEngine/InputManager.h>
#include <AGEngine/SpriteBatch.h>
#include <AGEngine/Timing.h>
#include <vector>
#include <map>
#include <unordered_set>
#include "Bullet.h"
#include "Agents.h"
#include "Player.h"
#include "CollisionDetection.h"

enum class GameState {PLAY, EXIT};

enum class Colours {
	RED, GREEN, BLUE, MAGENTA, WHITE, BLACK, ORANGE, YELLOW, L_BLUE, PINK, GREY
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

	std::map<Colours, AGEngine::Colour> allColours = {
		{Colours::RED, AGEngine::Colour{255, 0, 0, 255}},
		{Colours::GREEN, AGEngine::Colour{0, 255, 0, 255}},
		{Colours::BLUE, AGEngine::Colour{0, 0, 255, 255}},
		{Colours::MAGENTA, AGEngine::Colour{255, 0, 255, 255}},
		{Colours::WHITE, AGEngine::Colour{255, 255, 255, 255}},
		{Colours::BLACK, AGEngine::Colour{0, 0, 0, 255}},
		{Colours::ORANGE, AGEngine::Colour{255, 128, 0, 255}},
		{Colours::YELLOW, AGEngine::Colour{255, 255, 0, 255}},
		{Colours::L_BLUE, AGEngine::Colour{0, 128, 255, 255}},
		{Colours::PINK, AGEngine::Colour{255, 128, 255, 255}},
		{Colours::GREY, AGEngine::Colour{64, 64, 64, 255}},
	};

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame(std::unordered_set< std::shared_ptr<AABB> > &collisionObjs);

	int _windowWidth, _windowHeight;
	float _time, _maxFps, _fps;

	GameState _gameState;
	AGEngine::Window _window;
	AGEngine::GLSLProgram _textureProgram;
	AGEngine::InputManager _inputManager;
	AGEngine::Camera2D _camera;
	AGEngine::SpriteBatch _spriteBatch;
	AGEngine::FPSLimiter _fpsLimiter;

	Player _player;
	std::vector<Bullet> _bullets;
	std::vector<Human> _humans;
	std::vector<Zombie> _zombies;
	std::vector<Sprite> _walls;
	QuadTree _gameArea;
};

