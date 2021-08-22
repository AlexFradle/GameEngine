#include <iostream>
#include <string>
#include "MainGame.h"
#include <AGEngine/ErrorHandling.h>
#include <AGEngine/ResourceManager.h>

MainGame::MainGame() :  
	_screenWidth(1024), 
	_screenHeight(768),
	_gameState(GameState::PLAY), 
	_time(0),
	_maxFps(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame() {

}

void MainGame::run() {
	initSystems();
	gameLoop();
}

void MainGame::initSystems() {
	AGEngine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(_maxFps);
}

void MainGame::initShaders() {
	_colourProgram.compileShaders("shaders/colourShading.vert", "shaders/colourShading.frag");
	_colourProgram.addAttribute("vertexPos");
	_colourProgram.addAttribute("vertexCol");
	_colourProgram.addAttribute("vertexUV");
	_colourProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin();

		processInput();

		_time += 0.01;

		_camera.update();

		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update()) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		drawGame();

		_fps = _fpsLimiter.end();
		
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}

	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {

		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;

		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;

		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;

		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_w))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));

	if (_inputManager.isKeyPressed(SDLK_s))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));

	if (_inputManager.isKeyPressed(SDLK_a))
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));

	if (_inputManager.isKeyPressed(SDLK_d))
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));

	if (_inputManager.isKeyPressed(SDLK_q))
		_camera.setScale(_camera.getScale() + SCALE_SPEED);

	if (_inputManager.isKeyPressed(SDLK_e))
		_camera.setScale(_camera.getScale() - SCALE_SPEED);

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mc = _inputManager.getMouseCoords();
		mc = _camera.convertScreenToWorld(mc);
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mc - playerPosition;
		direction = glm::normalize(direction);
		_bullets.emplace_back(playerPosition, direction, 5.0f, 100);
	}
}

void MainGame::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colourProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colourProgram.getUniformLocation("mySampler");

	// Using GL_TEXTURE0, so 0 is the value of the sampler
	glUniform1i(textureLocation, 0);

	//GLuint timeLocation = _colourProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);

	GLuint pLocation = _colourProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static AGEngine::GLTexture texture = AGEngine::ResourceManager::getTexture("textures/man.png");
	AGEngine::Colour col;
	col.r = 255;
	col.g = 255;
	col.b = 255;
	col.a = 255;
	_spriteBatch.draw(pos, uv, texture.id, 0.0f, col);

	for (Bullet b : _bullets) {
		b.draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colourProgram.unuse();

	_window.swapBuffer();
}
