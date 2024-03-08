#include <sstream>
#include <string>
#include <memory>

#include "bsptree.h"
#include "random.h"

//////////////// generation

void BSPTree::generate(Dungeon* dungeon) {
	int width = dungeon->getWidth() - 1; // - 1 for right most wall
	int length = dungeon->getLength() - 1; // - 1 for lower most wall

	std::shared_ptr<Point> start = std::make_shared<Point>(0, 0);
	std::shared_ptr<Point> end = std::make_shared<Point>(width, length);
	Node root = Node(start, end);
	generate(root);
	setTiles(dungeon, root);
}

void BSPTree::generate(Node& node) {
	double orient = randomDouble();

	if (orient >= 0.5) {
		// is there enough space to split in the randomly chosen orientation
		if (hasEnoughSpace(node.getStartPoint()->getX(), node.getEndPoint()->getX(), 2, minRoomWidth)) {
			generateSplit(false, node);
		}
		// is there enough space to split in the other orientation
		else if (hasEnoughSpace(node.getStartPoint()->getY(), node.getEndPoint()->getY(), 2, minRoomLength, 6)) {
			generateSplit(true, node);
		}
		// this is a leaf node so generate a room
		else {
			generateRoom(node);
		}
	}
	else {
		// is there enough space to split in the randomly chosen orientation
		if (hasEnoughSpace(node.getStartPoint()->getY(), node.getEndPoint()->getY(), 2, minRoomLength)) {
			generateSplit(true, node);
		}
		// is there enough space to split in the other orientation
		else if (hasEnoughSpace(node.getStartPoint()->getX(), node.getEndPoint()->getX(), 2, minRoomWidth, 6)) {
			generateSplit(false, node);
		}
		// this is a leaf node so generate a room
		else {
			generateRoom(node);
		}
	}
}

void BSPTree::generateSplit(bool isVertical, Node& node) {
	Node* child1 = NULL;
	Node* child2 = NULL;

	// create children based on given split (vertical or horizontal)
	if (isVertical) {
		int splitAt = randomInt(node.getStartPoint()->getY() + minRoomWidth + 1, node.getEndPoint()->getY() - minRoomWidth - 1);

		std::shared_ptr<Point> splitStartPoint = std::make_shared<Point>(node.getStartPoint()->getX(), splitAt);
		std::shared_ptr<Point> splitEndPoint = std::make_shared<Point>(node.getEndPoint()->getX(), splitAt);
		child1 = new Node(node.getStartPoint(), splitEndPoint);
		child2 = new Node(splitStartPoint, node.getEndPoint());
	}
	else {
		int splitAt = randomInt(node.getStartPoint()->getX() + minRoomWidth + 1, node.getEndPoint()->getX() - minRoomWidth - 1);

		std::shared_ptr<Point> splitStartPoint = std::make_shared<Point>(splitAt, node.getStartPoint()->getY());
		std::shared_ptr<Point> splitEndPoint = std::make_shared<Point>(splitAt, node.getEndPoint()->getY());
		child1 = new Node(node.getStartPoint(), splitEndPoint);
		child2 = new Node(splitStartPoint, node.getEndPoint());
	}

	// set node children
	node.setChildren(isVertical, child1, child2);

	// recurse with children
	generate(*node.getFirstChild());
	generate(*node.getSecondChild());
}

void BSPTree::generateRoom(Node& node) {
	if (!hasEnoughSpace(node.getStartPoint()->getX(), node.getEndPoint()->getX(), 1, minRoomWidth) && (node.getStartPoint()->getY(), node.getEndPoint()->getY(), 1, minRoomLength)) {
		return;
	}

	Point* start = new Point(
		randomInt(node.getStartPoint()->getX() + 1, node.getEndPoint()->getX() - minRoomWidth + 1),
		randomInt(node.getStartPoint()->getY() + 1, node.getEndPoint()->getY() - minRoomLength + 1)
	);

	Point* end = new Point(
		randomInt(start->getX() + minRoomWidth, node.getEndPoint()->getX() + 1),
		randomInt(start->getY() + minRoomLength, node.getEndPoint()->getY() + 1)
	);

	// set node as leaf
	Room* room = new Room(start, end);
	node.setLeaf(room);
}

//////////////// helper generation functions

void swapNumsAscending(int& a, int& b) {
	if (b >= a) {
		return;
	}

	int c = a;
	a = b;
	b = c;
}

bool BSPTree::hasEnoughSpace(int lb, int ub, int numRooms, int minRoomLength, int extra) {
	return (ub - lb) >= minRoomLength * numRooms + numRooms + extra;
}

//////////////// set tiles

void BSPTree::setTiles(Dungeon* dungeon, Node& node) {
	setFloorTiles(dungeon, node);
	setCorridorTiles(dungeon, node);
	setWallTiles(dungeon, node);
}

void BSPTree::setFloorTiles(Dungeon* dungeon, Node& node) {
	// set floor tiles for room if this node has a room
	if (node.getRoom()) {
		setTiles(dungeon, *node.getRoom());
	}

	// recurse with children
	if (node.getFirstChild()) {
		setFloorTiles(dungeon, *node.getFirstChild());
	}

	if (node.getSecondChild()) {
		setFloorTiles(dungeon, *node.getSecondChild());
	}
}

void BSPTree::setCorridorTiles(Dungeon* dungeon, Node& node) {
	// exit if reached leaf
	if (!node.getFirstChild() && !node.getSecondChild()) {
		return;
	}

	// connect every pair of children (ensures full connectivity between rooms)
	Room& firstRoom = node.getFirstChild()->getAnyRoom();
	Room& secondRoom = node.getSecondChild()->getAnyRoom();

	// get middle point from both rooms
	Point firstMiddlePoint = Point(0, 0);
	firstRoom.getMiddlePoint(firstMiddlePoint);
	Point secondMiddlePoint = Point(0, 0);
	secondRoom.getMiddlePoint(secondMiddlePoint);

	// create L shaped corridor by creating corridor bend point
	double direction = randomDouble();
	bool verticalFirst = false;
	Point bendPoint = Point(0, 0);
	if (direction >= 0.5) {
		bendPoint = Point(firstMiddlePoint.getX(), secondMiddlePoint.getY());
		verticalFirst = true;
	}
	else {
		bendPoint = Point(secondMiddlePoint.getX(), firstMiddlePoint.getY());
	}

	// loop over corridor points to set tiles
	if (verticalFirst) {
		int firstY = firstMiddlePoint.getY();
		int secondY = bendPoint.getY();
		swapNumsAscending(firstY, secondY);
		for (int y = firstY; y <= secondY; y++) {
			dungeon->setTile(floorTile, firstMiddlePoint.getX(), y);
		}

		int firstX = bendPoint.getX();
		int secondX = secondMiddlePoint.getX();
		swapNumsAscending(firstX, secondX);
		for (int x = firstX; x <= secondX; x++) {
			dungeon->setTile(floorTile, x, secondMiddlePoint.getY());
		}
	}
	else {
		int firstX = firstMiddlePoint.getX();
		int secondX = bendPoint.getX();
		swapNumsAscending(firstX, secondX);
		for (int x = firstX; x <= secondX; x++) {
			dungeon->setTile(floorTile, x, firstMiddlePoint.getY());
		}

		int firstY = bendPoint.getY();
		int secondY = secondMiddlePoint.getY();
		swapNumsAscending(firstY, secondY);
		for (int y = firstY; y <= secondY; y++) {
			dungeon->setTile(floorTile, secondMiddlePoint.getX(), y);
		}
	}

	// recurse with children
	setCorridorTiles(dungeon, *node.getFirstChild());
	setCorridorTiles(dungeon, *node.getSecondChild());
}

void BSPTree::setWallTiles(Dungeon* dungeon, Node& node) {
	for (int y = node.getStartPoint()->getY(); y < node.getEndPoint()->getY(); y++) {
		for (int x = node.getStartPoint()->getX(); x < node.getEndPoint()->getX(); x++) {
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

void BSPTree::setTiles(Dungeon* dungeon, Room& room) {
	for (int y = room.getStartPoint().getY(); y < room.getEndPoint().getY(); y++) {
		for (int x = room.getStartPoint().getX(); x < room.getEndPoint().getX(); x++) {
			dungeon->setTile(floorTile, x, y);
		}
	}
}
