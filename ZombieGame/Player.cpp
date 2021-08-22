#include "Player.h"
#include <iostream>

Player::Player() {

}

Player::Player(glm::vec2 pos, float width, float height, GLuint textureID, AGEngine::Colour colour)
	: Sprite(pos, width, height, textureID, colour)
{
	gun = Gun(pos.x + (width / 2), pos.y + (height / 2));
}

Player::~Player() {

}

void Player::move(glm::vec2 moveAmount) {
	this->move(moveAmount.x, moveAmount.y);
}

void Player::move(float moveAmountX, float moveAmountY) {
	glm::vec2 curPos = this->getPosition();
	this->setPosition(glm::vec2(curPos.x + moveAmountX, curPos.y + moveAmountY));
}
