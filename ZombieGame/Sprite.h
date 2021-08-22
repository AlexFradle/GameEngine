#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <AGEngine/Vertex.h>
#include <AGEngine/SpriteBatch.h>
#include <memory>
#include "CollisionDetection.h"

class Sprite
{
public:
	Sprite();
	Sprite(glm::vec2 pos, float width, float height, GLuint textureID, AGEngine::Colour colour);
	~Sprite();

	void draw(AGEngine::SpriteBatch &spriteBatch);
	bool collideSprite(Sprite *otherSprite);
	//bool collideAABB(AABB )
	
	// Getters
	glm::vec2 getPosition() { return glm::vec2(_position.x, _position.y); }
	glm::vec4 getPositionAndSize() { return glm::vec4(_position.x, _position.y, _width, _height); }
	glm::vec2 getCenterCoords() { return glm::vec2(_position.x + (_width / 2), _position.y + (_height / 2)); }
	std::shared_ptr<AABB> getBoundary() { return _boundary; }

	// Setters
	void setPosition(glm::vec2 newPos) { _position = newPos; _boundary->setCenter(getCenterCoords()); }
	void setPosition(float newX, float newY) { _position = glm::vec2(newX, newY); _boundary->setCenter(getCenterCoords()); }

private:
	glm::vec2 _position;
	GLuint _textureID;
	float _width, _height;
	AGEngine::Colour _colour;
	glm::vec4 _uv;
	std::shared_ptr<AABB> _boundary;
};

