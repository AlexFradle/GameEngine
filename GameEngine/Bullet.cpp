#include "Bullet.h"
#include <AGEngine/ResourceManager.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime) {
	_position = pos;
	_direction = dir;
	_speed = speed;
	_lifeTime = lifeTime;
}

Bullet::~Bullet() {

}

void Bullet::draw(AGEngine::SpriteBatch &spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static AGEngine::GLTexture texture = AGEngine::ResourceManager::getTexture("textures/man.png");
	AGEngine::Colour col;
	col.r = 255;
	col.g = 255;
	col.b = 255;
	col.a = 255;
	
	glm::vec4 posAndSize(_position.x, _position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, col);
}

bool Bullet::update() {
	// increment the position by the unit vector * speed
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0)
		return true;
	
	return false;
}
