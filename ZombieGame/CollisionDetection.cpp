#include "CollisionDetection.h"
#include <iostream>

XY::XY() {
	_x = 8008.0f;
}

XY::XY(float newX, float newY) {
	_x = newX;
	_y = newY;
}

AABB::AABB() {

}

AABB::AABB(float centerX, float centerY, float halfW, float halfH) {
	_center = XY(centerX, centerY);
	_halfW = halfW;
	_halfH = halfH;
	setSides();
}

void AABB::setPoints() {
	//https://stackoverflow.com/questions/11711034/stdshared-ptr-of-this

	_points[0] = XY(_leftSide, _topSide);
	_points[0].setParent(shared_from_this());

	_points[1] = XY(_leftSide, _bottomSide);
	_points[1].setParent(shared_from_this());

	_points[2] = XY(_rightSide, _topSide);
	_points[2].setParent(shared_from_this());

	_points[3] = XY(_rightSide, _bottomSide);
	_points[3].setParent(shared_from_this());

}

void AABB::setSides() {
	_leftSide = _center.getX() - _halfW;
	_rightSide = _center.getX() + _halfW;
	_topSide = _center.getY() + _halfH;
	_bottomSide = _center.getY() - _halfH;
}

bool AABB::intersects(std::shared_ptr<AABB> other) {
	return (
		(_leftSide < other->getRightSide()) &&
		(_rightSide > other->getLeftSide()) &&
		(_topSide > other->getBottomSide()) &&
		(_bottomSide < other->getTopSide())
	);
}

bool AABB::intersects(AABB *other) {
	return (
		(_leftSide < other->getRightSide()) &&
		(_rightSide > other->getLeftSide()) &&
		(_topSide > other->getBottomSide()) &&
		(_bottomSide < other->getTopSide())
	);
}

bool AABB::containsPoint(XY *p) {
	return (
		(p->getX() >= _leftSide) &&
		(p->getX() <= _rightSide) &&
		(p->getY() <= _topSide) &&
		(p->getY() >= _bottomSide)
	);
}

void AABB::insertIntoQuadTree(QuadTree &qt) {
	for (XY &point : _points) {
		qt.insert(&point);
	}
}

QuadTree::QuadTree() {

}

QuadTree::QuadTree(AABB boundary) {
	_boundary = boundary;
	_northWest = nullptr;
	_northEast = nullptr;
	_southWest = nullptr;
	_southEast = nullptr;
}

QuadTree::~QuadTree() {

}

bool QuadTree::insert(XY *p) {
	if (!_boundary.containsPoint(p) || !_boundary.intersects(p->getParent())) {
		return false;
	}

	if (_points.size() < QT_NODE_CAPACITY && _northWest == nullptr) {
		_points.push_back(p);
		return true;
	}

	if (_northWest == nullptr)
		subdivide();

	if (_northWest->insert(p)) return true;
	if (_northEast->insert(p)) return true;
	if (_southWest->insert(p)) return true;
	if (_southEast->insert(p)) return true;
}

void QuadTree::subdivide() {
	float halfW = _boundary.getHalfWidth() / 2;
	float halfH = _boundary.getHalfHeight() / 2;

	_northWest = new QuadTree(
		AABB(_boundary.getCenterX() - halfW, _boundary.getCenterY() + halfH, halfW, halfH)
	);
	_northEast = new QuadTree(
		AABB(_boundary.getCenterX() + halfW, _boundary.getCenterY() + halfH, halfW, halfH)
	);
	_southWest = new QuadTree(
		AABB(_boundary.getCenterX() - halfW, _boundary.getCenterY() - halfH, halfW, halfH)
	);
	_southEast = new QuadTree(
		AABB(_boundary.getCenterX() + halfW, _boundary.getCenterY() - halfH, halfW, halfH)
	);
}

void QuadTree::queryRange(AABB *range, std::vector< XY * > *pointsInRange) {
	if (!_boundary.intersects(range))
		return;

	for (auto p : _points) {
		if (range->containsPoint(p) || range->intersects(p->getParent())) {
			pointsInRange->push_back(p);
		}
	}

	if (_northWest == nullptr)
		return;

	_northWest->queryRange(range, pointsInRange);
	_northEast->queryRange(range, pointsInRange);
	_southWest->queryRange(range, pointsInRange);
	_southEast->queryRange(range, pointsInRange);

	return;
}

void QuadTree::clear() {
	_points.clear();
	
	if (_northWest != nullptr) {
		_northWest->clear();
		delete _northWest;
		_northWest = nullptr;
	}
	
	if (_northEast != nullptr) {
		_northEast->clear();
		delete _northEast;
		_northEast = nullptr;
	}

	if (_southWest != nullptr) {
		_southWest->clear();
		delete _southWest;
		_southWest = nullptr;
	}

	if (_southEast != nullptr) {
		_southEast->clear();
		delete _southEast;
		_southEast = nullptr;
	}
}
