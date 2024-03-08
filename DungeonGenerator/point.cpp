#include <sstream>
#include <string>
#include <memory>

#include "point.h"

// getters and setters
int Point::getX() { return x; }
int Point::getY() { return y; }
void Point::setX(int x) { this->x = x; }
void Point::setY(int y) { this->y = y; }

void Point::add(Point point) {
	x += point.getX();
	y += point.getY();
}

void Point::subtract(Point point) {
	x -= point.getX();
	y -= point.getY();
}

std::string Point::toString() {
	std::ostringstream oss;
	oss << "Point(" << x << ", " << y << ")";
	std::string str = oss.str();

	return str;
}