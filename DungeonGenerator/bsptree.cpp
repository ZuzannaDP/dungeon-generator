#include <sstream>
#include <string>

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
	std::cout << "set tiles \n";
	setTiles(dungeon, root);
}

void BSPTree::generate(Node& node) {
	std::cout << "at address " << &node << "\n";
	std::cout << "at " << (*node.getStartPoint()).toString() << ", " << (*node.getEndPoint()).toString() << "\n";

	double orient = randomDouble();

	if (orient >= 0.5) {
		// is there enough space to split in the randomly chosen orientation
		if (hasEnoughSpaceToSplit(node.getStartPoint()->getX(), node.getEndPoint()->getX(), minRoomWidth)) {
			generateSplit(false, node);
		}
		// is there enough space to split in the other orientation
		else if (hasEnoughSpaceToSplit(node.getStartPoint()->getY(), node.getEndPoint()->getY(), minRoomLength, 6)) {
			generateSplit(true, node);
		}
		// this is a leaf node so generate a room
		else {
			generateRoom(node);
		}
	}
	else {
		// is there enough space to split in the randomly chosen orientation
		if (hasEnoughSpaceToSplit(node.getStartPoint()->getY(), node.getEndPoint()->getY(), minRoomLength)) {
			generateSplit(true, node);
		}
		// is there enough space to split in the other orientation
		else if (hasEnoughSpaceToSplit(node.getStartPoint()->getX(), node.getEndPoint()->getX(), minRoomWidth, 6)) {
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

	std::cout << "child1 addresses " << child1->getStartPoint() << ", " << child1->getEndPoint() << "\n";
	std::cout << "child2 addresses " << child2->getStartPoint() << ", " << child2->getEndPoint() << "\n";
	std::cout << "child1 " << child1->getStartPoint()->toString() << ", " << child1->getEndPoint()->toString() << "\n";
	std::cout << "child2 " << child2->getStartPoint()->toString() << ", " << child2->getEndPoint()->toString() << "\n";

	// recurse with children
	generate(*node.getFirstChild());
	generate(*node.getSecondChild());
}

void BSPTree::generateRoom(Node& node) {
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

bool BSPTree::hasEnoughSpaceToSplit(int lb, int ub, int minRoomLength, int extra) {
	return (ub - lb) >= minRoomLength * 2 + 2 + extra;
}

//////////////// set tiles

void BSPTree::setTiles(Dungeon* dungeon, Node& node) {
	std::cout << "at address " << &node;
	std::cout << "at " << (*node.getStartPoint()).toString() << ", " << (*node.getEndPoint()).toString() << "\n";

	std::cout << (*node.getStartPoint()).toString() << "\n";
	std::cout << (*node.getEndPoint()).toString() << "\n";

	setFloorTiles(dungeon, node);
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

void BSPTree::setTiles(Dungeon* dungeon, Room room) {
	for (int y = room.getStartPoint().getY(); y < room.getEndPoint().getY(); y++) {
		for (int x = room.getStartPoint().getX(); x < room.getEndPoint().getX(); x++) {
			dungeon->setTile(floorTile, x, y);
		}
	}
}
