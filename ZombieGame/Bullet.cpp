#include "Bullet.h"

Bullet::Bullet(
	glm::vec2 pos, float width, float height, GLuint textureID, 
	AGEngine::Colour colour, glm::vec2 direction, float speed, float lifeTime
) : Sprite(pos, width, height, textureID, colour)
{
	_direction = direction;
	_speed = speed;
	_lifeTime = lifeTime;
}

Bullet::~Bullet() {

}

bool Bullet::update() {
	// increment the position by the unit vector * speed
	this->setPosition(this->getPosition() + (_direction * _speed));
	_lifeTime--;
	if (_lifeTime == 0)
		return true;

	return false;
}
