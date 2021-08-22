#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <memory>

class AABB;
class XY;
class QuadTree;

class XY {
public:
	XY();
	XY(float newX, float newY);

	float getX() { return _x; }
	float getY() { return _y; }
	std::shared_ptr<AABB> getParent() { return _parent; }

	void setX(float newX) { _x = newX; }
	void setY(float newY) { _y = newY; }
	void setParent(std::shared_ptr<AABB> parent) { _parent = parent; }

private:
	float _x, _y;
	std::shared_ptr<AABB> _parent;
};

class AABB : public std::enable_shared_from_this<AABB> {
public:
	AABB();
	AABB(float centerX, float centerY, float halfW, float halfH);

	bool intersects(std::shared_ptr<AABB> other);
	bool intersects(AABB *other);
	bool containsPoint(XY *p);
	void insertIntoQuadTree(QuadTree &qt);
	void setPoints();

	// Getters
	XY *getCenter() { return &_center; }
	float getCenterX() { return _center.getX(); }
	float getCenterY() { return _center.getY(); }
	float getLeftSide() { return _leftSide; }
	float getRightSide() { return _rightSide; }
	float getTopSide() { return _topSide; }
	float getBottomSide() { return _bottomSide; }
	float getHalfWidth() { return _halfW; }
	float getHalfHeight() { return _halfH; }

	// Setters
	void setCenter(glm::vec2 newCoords) { _center.setX(newCoords.x); _center.setY(newCoords.y); setSides(); }
	void setCenter(float newX, float newY) { _center.setX(newX); _center.setY(newY); setSides(); }

private:
	void setSides();

	XY _center;
	float _halfW, _halfH, _leftSide, _rightSide, _topSide, _bottomSide;
	std::array<XY, 4> _points;
};

class QuadTree
{
public:
	QuadTree();
	QuadTree(AABB boundary);
	~QuadTree();

	bool insert(XY *p);
	void queryRange(AABB *range, std::vector< XY * > *pointsInRange);
	void clear();

	static const int QT_NODE_CAPACITY = 4;
	
private:
	void subdivide();

	AABB _boundary;
	std::vector< XY * > _points;
	QuadTree *_northWest;
	QuadTree *_northEast;
	QuadTree *_southWest;
	QuadTree *_southEast;
};

