#ifndef POINT_H
#define POINT_H

#include <string>

class Point
{
private:
	int x;
	int y;

public:
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int getX();
	int getY();
	void setX(int x);
	void setY(int y);

	/*
	 In place addition.
	 @param point: The point to add.
	 */
	void add(Point point);

	/*
	 In place subtraction.
	 @param point: The point to subtract.
	 */
	void subtract(Point point);

	/*
	 Get string representation of the point.
	 */
	std::string toString();
};

#endif
