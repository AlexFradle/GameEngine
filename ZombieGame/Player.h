#pragma once
#include "Sprite.h"
#include "Gun.h"

class Player : public Sprite
{
public:
	Player();
	Player(glm::vec2 pos, float width, float height, GLuint textureID, AGEngine::Colour colour);
	~Player();

	void move(glm::vec2 moveAmount);
	void move(float moveAmountX, float moveAmountY);

	Gun gun;

private:


};

