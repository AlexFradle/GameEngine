#include "MainGame.h"
#include <SDL/SDL.h>
#include <AGEngine/AGEngine.h>
#include <glm/glm.hpp>
#include <AGEngine/ResourceManager.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

MainGame::MainGame()
{
	_windowWidth = 1024;
	_windowHeight = 768;
	_time = 0.0f;
	_maxFps = 60.0f;
	_gameState = GameState::PLAY;
	_camera.init(_windowWidth, _windowHeight);
}

MainGame::~MainGame() {

}

void MainGame::run() {
	initSystems();
	gameLoop();
}

void MainGame::initSystems() {
	AGEngine::init();
	_window.create("Zombie Game", _windowWidth, _windowHeight, allColours[Colours::GREY], 0);
	initShaders();
	_spriteBatch.init();
	float w = 50.0f, h = 50.0f;
	_player = Player(glm::vec2(-(w / 2), -(h / 2)), w, h,
		AGEngine::ResourceManager::getTexture("textures/circle.png").id,
		allColours[Colours::BLUE]
	);
	_fpsLimiter.init(_maxFps);
	
	std::vector<std::string> lines;
	std::string line;
	std::ifstream file("map.txt");
	int lineCount = 0;
	while (std::getline(file, line)) {
		lines.push_back(line);
		lineCount++;
	}
	file.close();

	//std::reverse(lines.begin(), lines.end());
	float wallWidth = 50.0f, wallHeight = 50.0f;

	for (std::string l : lines) {
		for (int i = 0; i < l.size(); i++) {
			std::string textureName = "";
			switch (l[i]) {
			case 'R':
				textureName = "red_bricks";
				break;

			case 'L':
				textureName = "light_bricks";
				break;

			case 'G':
				textureName = "glass";
				break;
			}

			if (textureName != "") {
				_walls.emplace_back(
					glm::vec2(i * wallWidth, lineCount * wallHeight), wallWidth, wallHeight,
					AGEngine::ResourceManager::getTexture("textures/" + textureName + ".png").id,
					allColours[Colours::WHITE]
				);
			}
		}
		lineCount--;
	}
	std::reverse(_walls.begin(), _walls.end());
	float gameWidth = 52.0f * 50.0f;
	float gameHeight = 33.0f * 50.0f;
	_gameArea = QuadTree(AABB(gameWidth / 2, gameHeight / 2, gameWidth / 2, gameHeight / 2));
}

void MainGame::initShaders() {
	_textureProgram.compileShaders("shaders/colourShading.vert", "shaders/colourShading.frag");
	_textureProgram.addAttribute("vertexPos");
	_textureProgram.addAttribute("vertexCol");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	std::vector< XY * > inRange = {};
	std::unordered_set< std::shared_ptr<AABB> > ableToCollideWith = {};
	while (_gameState != GameState::EXIT) {
		// Get the time at the start of frame
		_fpsLimiter.begin();
		
		// Check for user input
		processInput();

		// increment the time
		_time += 0.01f;

		// 
		_camera.update();

		//////////////// GAME LOGIC AREA ////////////////////////////
		_player.gun.update(_player.getBoundary()->getCenterX(), _player.getBoundary()->getCenterY());

		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update()) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		_gameArea.clear();
		for (auto &s : _walls) {
			s.getBoundary()->insertIntoQuadTree(_gameArea);
		}

		inRange.clear();
		ableToCollideWith.clear();
		_gameArea.queryRange(_player.gun.getArea(), &inRange);
		for (XY *pointPtr : inRange) {
			ableToCollideWith.insert(pointPtr->getParent());
		}
		/////////////////////////////////////////////////////////////

		//
		drawGame(ableToCollideWith);

		// Get time at the end of the frame and process the fps
		_fps = _fpsLimiter.end();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	const float CAMERA_SPEED = 5.0f;
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

	glm::vec2 moveAmount(0.0f);
	if (_inputManager.isKeyPressed(SDLK_w)) {
		moveAmount = glm::vec2(0.0f, CAMERA_SPEED);
		_camera.setPosition(_camera.getPosition() + moveAmount);
		_player.move(moveAmount);
	}

	if (_inputManager.isKeyPressed(SDLK_s)) {
		moveAmount = glm::vec2(0.0f, -CAMERA_SPEED);
		_camera.setPosition(_camera.getPosition() + moveAmount);
		_player.move(moveAmount);
	}

	if (_inputManager.isKeyPressed(SDLK_a)) {
		moveAmount = glm::vec2(-CAMERA_SPEED, 0.0f);
		_camera.setPosition(_camera.getPosition() + moveAmount);
		_player.move(moveAmount);
	}

	if (_inputManager.isKeyPressed(SDLK_d)) {
		moveAmount = glm::vec2(CAMERA_SPEED, 0.0f);
		_camera.setPosition(_camera.getPosition() + moveAmount);
		_player.move(moveAmount);
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mc = _inputManager.getMouseCoords();
		mc = _camera.convertScreenToWorld(mc);
		glm::vec2 playerPosition = _player.getCenterCoords();
		glm::vec2 direction = glm::normalize(mc - playerPosition);
		_bullets.emplace_back(
			playerPosition, 10.0f, 10.0f, 
			AGEngine::ResourceManager::getTexture("textures/circle.png").id, 
			allColours[Colours::RED], direction, 5.0f, 100.0f
		);
	}
	if (_inputManager.isKeyPressed(SDLK_q))
		_camera.setScale(_camera.getScale() + SCALE_SPEED);

	if (_inputManager.isKeyPressed(SDLK_e))
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
}

void MainGame::drawGame(std::unordered_set< std::shared_ptr<AABB> > &collisionObjs) {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);
	GLint pLocation = _textureProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	for (Bullet &b : _bullets)
		b.draw(_spriteBatch);

	_player.draw(_spriteBatch);

	for (Sprite &w : _walls)
		w.draw(_spriteBatch);

	std::cout << collisionObjs.size() << std::endl;
	for (auto &atcw : collisionObjs) {
		Sprite newSprite = Sprite(
			glm::vec2(atcw->getLeftSide(), atcw->getBottomSide()), atcw->getHalfWidth() * 2, atcw->getHalfHeight() * 2,
			AGEngine::ResourceManager::getTexture("textures/glass.png").id,
			allColours[Colours::GREEN]
		);
		newSprite.draw(_spriteBatch);
	}
	
	/*Sprite gArea = Sprite(
		glm::vec2(_player.gun.getArea()->getLeftSide(), _player.gun.getArea()->getBottomSide()), 
		_player.gun.getArea()->getHalfWidth() * 2, _player.gun.getArea()->getHalfHeight() * 2,
		AGEngine::ResourceManager::getTexture("textures/glass.png").id,
		allColours[Colours::RED]
	);
	gArea.draw(_spriteBatch);*/

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_textureProgram.unuse();
	_window.swapBuffer();
}
