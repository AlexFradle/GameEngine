#include "Sprite.h"
#include <iostream>

Sprite::Sprite() {

}

Sprite::Sprite(glm::vec2 pos, float width, float height, GLuint textureID, AGEngine::Colour colour) {
	_position = pos;
	_width = width;
	_height = height;
	_textureID = textureID;
	_colour = colour;
	// texture covers all surface
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	_boundary = std::make_shared<AABB>(_position.x + (_width / 2), _position.y + (_height / 2), _width / 2, _height / 2);
	_boundary->setPoints();
}

Sprite::~Sprite() {

}

void Sprite::draw(AGEngine::SpriteBatch &spriteBatch) {
	spriteBatch.draw(getPositionAndSize(), _uv, _textureID, 0.0f, _colour);
}

bool Sprite::collideSprite(Sprite *otherSprite) {
	return false;
}
