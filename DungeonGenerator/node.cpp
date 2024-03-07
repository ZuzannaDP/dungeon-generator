#include <sstream>
#include <string>

#include "node.h"
#include "random.h"

//////////////// setters and getters 
// (some pass by reference because null is not possible, others pass pointer because null is possible)

Point& Room::getStartPoint() { return *startCorner; }
Point& Room::getEndPoint() { return *endCorner; }
std::shared_ptr<Point> Node::getStartPoint() { return startCorner; }
std::shared_ptr<Point> Node::getEndPoint() { return endCorner; }
Node* Node::getFirstChild() { return child1; }
Node* Node::getSecondChild() { return child2; }
Room* Node::getRoom() { return room; }
bool Node::isVerticalSplit() { return vertical; }

void Node::setChildren(bool isVertical, Node* child1, Node* child2) {
	if (!room) {
		this->child1 = child1;
		this->child2 = child2;
	}
}

void Node::setLeaf(Room* room) {
	if (!child1 && !child2) {
		this->room = room;
	}
}

//////////////// to string

std::string Node::toString() {
	std::ostringstream oss;
	oss << "Node (" << startCorner->toString() << " to " << endCorner->toString() << ") \n";
	std::string str = oss.str();

	return str;
}

std::string Room::toString() {
	std::ostringstream oss;
	oss << "Room (" << startCorner->toString() << " to " << endCorner->toString() << ") \n";
	std::string str = oss.str();

	return str;
}