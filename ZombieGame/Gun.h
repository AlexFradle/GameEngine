#pragma once
#include "CollisionDetection.h"

class Gun
{
public:
	Gun();
	Gun(float centerX, float centerY);

	void update(float newCenterX, float newCenterY);

	AABB *getArea() { return &_area; }

private:
	AABB _area;
};

