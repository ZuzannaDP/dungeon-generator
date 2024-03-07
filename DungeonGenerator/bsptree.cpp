#include <sstream>
#include <string>

#include "bsptree.h"

//////////////// random

/*
 Get a random double between 0 and 1.
 */
double randomDouble() {
	return ((double)std::rand() / (double)RAND_MAX);
}

/*
 Get a random integer.
 @param from: From this value (inclusive).
 @param to: To this value (exclusive).
 */
int randomInt(int from, int to) {
	if (to <= from) { return from; }

	int range = to - from;
	return from + (rand() % range);
}

//////////////// generation

void BSPTree::generate(Dungeon* dungeon) {
	int width = dungeon->getWidth() - 1; // - 1 for right most wall
	int length = dungeon->getLength() - 1; // - 1 for lower most wall

	std::shared_ptr<Point> start = std::make_shared<Point>(0, 0);
	std::shared_ptr<Point> end = std::make_shared<Point>(width, length);
	Node root = Node(start, end);
	root.generate();

	root.setTiles(dungeon);
}

void Node::generate() {
	double orient = randomDouble();

	if (orient >= 0.5) {
		// is there enough space to split in the randomly chosen orientation
		if (hasEnoughSpaceToSplit(startCorner->getX(), endCorner->getX(), minRoomWidth)) {
			generateHorizontalSplit();
		}
		// is there enough space to split in the other orientation
		else if (hasEnoughSpaceToSplit(startCorner->getY(), endCorner->getY(), minRoomLength, 6)) {
			generateVerticalSplit();
		}
		// this is a leaf node so generate a room
		else {
			generateRoom();
		}
	}
	else {
		// is there enough space to split in the randomly chosen orientation
		if (hasEnoughSpaceToSplit(startCorner->getY(), endCorner->getY(), minRoomLength)) {
			generateVerticalSplit();
		}
		// is there enough space to split in the other orientation
		else if (hasEnoughSpaceToSplit(startCorner->getX(), endCorner->getX(), minRoomWidth, 6)) {
			generateHorizontalSplit();
		}
		// this is a leaf node so generate a room
		else {
			generateRoom();
		}
	}
}

void Node::generateHorizontalSplit() {
	vertical = false;

	// create a horizontal split
	int splitAt = randomInt(startCorner->getX() + minRoomWidth + 1, endCorner->getX() - minRoomWidth - 1);

	// create two children
	std::shared_ptr<Point> splitStartPoint = std::make_shared<Point>(splitAt, startCorner->getY());
	std::shared_ptr<Point> splitEndPoint = std::make_shared<Point>(splitAt, endCorner->getY());
	this->child1 = new Node(startCorner, splitEndPoint);
	this->child2 = new Node(splitStartPoint, endCorner);

	// recurse with children
	child1->generate();
	child2->generate();
}

void Node::generateVerticalSplit() {
	vertical = true;

	// create a horizontal split
	int splitAt = randomInt(startCorner->getY() + minRoomWidth + 1, endCorner->getY() - minRoomWidth - 1);

	// create two children
	std::shared_ptr<Point> splitStartPoint = std::make_shared<Point>(startCorner->getX(), splitAt);
	std::shared_ptr<Point> splitEndPoint = std::make_shared<Point>(endCorner->getX(), splitAt);
	this->child1 = new Node(startCorner, splitEndPoint);
	this->child2 = new Node(splitStartPoint, endCorner);

	// recurse with children
	child1->generate();
	child2->generate();
}

void Node::generateRoom() {
	Point* start = new Point(
		randomInt(startCorner->getX() + 1, endCorner->getX() - minRoomWidth + 1),
		randomInt(startCorner->getY() + 1, endCorner->getY() - minRoomLength + 1)
	);

	Point* end = new Point(
		randomInt(start->getX() + minRoomWidth, endCorner->getX() + 1),
		randomInt(start->getY() + minRoomLength, endCorner->getY() + 1)
	);

	this->room = new Room(start, end);
}

//////////////// helper generation functions

bool Node::hasEnoughSpaceToSplit(int lb, int ub, int minRoomLength, int extra) {
	return (ub - lb) >= minRoomLength * 2 + 2 + extra;
}

//////////////// set tiles

void Node::setTiles(Dungeon* dungeon) {
	setFloorTiles(dungeon);
	setWallTiles(dungeon);
}

void Node::setFloorTiles(Dungeon* dungeon) {
	// set floor tiles for room if this node has a room
	if (room) {
		room->setTiles(dungeon);
	}

	// recurse with children
	if (child1) {
		child1->setFloorTiles(dungeon);
	}

	if (child2) {
		child2->setFloorTiles(dungeon);
	}
}

void Node::setWallTiles(Dungeon* dungeon) {
	for (int y = startCorner->getY(); y < endCorner->getY(); y++) {
		for (int x = startCorner->getX(); x < endCorner->getX(); x++) {
			if (dungeon->getTile(x, y) == floorTile) {
				// set surrounding void tiles to walls (going clockwise)
				if (dungeon->getTile(x - 1, y - 1) == voidTile) {
					dungeon->setTile(wallTile, x - 1, y - 1);
				}
				if (dungeon->getTile(x, y - 1) == voidTile) {
					dungeon->setTile(wallTile, x, y - 1);
				}
				if (dungeon->getTile(x + 1, y - 1) == voidTile) {
					dungeon->setTile(wallTile, x + 1, y - 1);
				}
				if (dungeon->getTile(x + 1, y) == voidTile) {
					dungeon->setTile(wallTile, x + 1, y);
				}
				if (dungeon->getTile(x + 1, y + 1) == voidTile) {
					dungeon->setTile(wallTile, x + 1, y + 1);
				}
				if (dungeon->getTile(x, y + 1) == voidTile) {
					dungeon->setTile(wallTile, x, y + 1);
				}
				if (dungeon->getTile(x - 1, y + 1) == voidTile) {
					dungeon->setTile(wallTile, x - 1, y + 1);
				}
				if (dungeon->getTile(x - 1, y) == voidTile) {
					dungeon->setTile(wallTile, x - 1, y);
				}
			}
		}
	}
}

void Room::setTiles(Dungeon* dungeon) {
	for (int y = startCorner->getY(); y < endCorner->getY(); y++) {
		for (int x = startCorner->getX(); x < endCorner->getX(); x++) {
			dungeon->setTile(floorTile, x, y);
		}
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