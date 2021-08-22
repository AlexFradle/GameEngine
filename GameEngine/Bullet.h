#pragma once
#include <glm/glm.hpp>
#include <AGEngine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(AGEngine::SpriteBatch &spriteBatch);
	bool update();  // returns true when it is end of life

private:
	float _speed;
	int _lifeTime;
	glm::vec2 _direction;
	glm::vec2 _position;
};

