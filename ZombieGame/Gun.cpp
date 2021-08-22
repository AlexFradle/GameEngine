#include "Gun.h"

Gun::Gun() {

}

Gun::Gun(float centerX, float centerY) {
	_area = AABB(centerX, centerY, 100.0f, 100.0f);
}

void Gun::update(float newCenterX, float newCenterY) {
	_area.setCenter(newCenterX, newCenterY);
}
