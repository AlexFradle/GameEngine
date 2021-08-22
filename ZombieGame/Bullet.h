#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <AGEngine/Vertex.h>
#include <AGEngine/SpriteBatch.h>
#include "Sprite.h"

class Bullet : public Sprite
{
public:
	Bullet(
		glm::vec2 pos, float width, float height, GLuint textureID, 
		AGEngine::Colour colour, glm::vec2 direction, float speed, float lifeTime
	);
	~Bullet();

	bool update();

private:
	glm::vec2 _direction;
	float _speed;
	int _lifeTime;
};

