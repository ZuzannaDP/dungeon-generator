#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <cstdlib>

#include "point.h"
#include "dungeon.h"

class Room
{
private:
	Point* startCorner;
	Point* endCorner;

public:
	Room(Point* startCorner, Point* endCorner) {
		this->startCorner = startCorner;
		this->endCorner = endCorner;
	}

	~Room() {
		delete startCorner;
		delete endCorner;
	}

	Point& getStartPoint();
	Point& getEndPoint();

	void getMiddlePoint(Point& point);

	/*
	 Get string representation of the room.
	 */
	std::string toString();
};

class Node
{
private:
	std::shared_ptr<Point> startCorner;
	std::shared_ptr<Point> endCorner;
	Node* child1;
	Node* child2;
	Room* room;
	bool vertical;

public:
	Node(std::shared_ptr<Point> startCorner, std::shared_ptr<Point> endCorner) {
		this->startCorner = startCorner;
		this->endCorner = endCorner;
		this->room = NULL;
		this->child1 = NULL;
		this->child2 = NULL;
		this->vertical = false;
	}

	~Node() {
		delete child1;
		delete child2;
		delete room;
	}

	std::shared_ptr<Point> getStartPoint();
	std::shared_ptr<Point> getEndPoint();
	Node* getFirstChild();
	Node* getSecondChild();
	Room* getRoom();
	bool isVerticalSplit();
	void setChildren(bool isVertical, Node* child1, Node* child2);
	void setLeaf(Room* room);

	/*
	 Get any room in this sub tree.
	 */
	Room& getAnyRoom();

	/*
	 Get string representation of the node.
	 */
	std::string toString();
};

#endif