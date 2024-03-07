#ifndef BSPTREE_H
#define BSPTREE_H

#include <stdio.h>
#include <cstdlib>

#include "point.h"
#include "dungeon.h"

// start point is always inclusive
// end point is always exclusive

const int minRoomWidth = 4;
const int minRoomLength = 4;

double randomDouble();

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

	/*
	 Set the tiles of the dungeon according to the 
	 start and end corners of the room.
	 */
	void setTiles(Dungeon* dungeon);

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

	/*
	 Checks if the node has enough space to split in one dimension. 
	 It has enough space if there is space for 2 rooms of minimum 
	 length, 2 tiles to ensure rooms don't touch, and any extra tiles.
	 @param lb: The lower bound of the space (inclusive).
	 @param ub: The upper bound of the space (exclusive).
	 @param minRoomLength: The minimum length of the room.
	 @param extra: Extra space to account for.
	 */
	bool hasEnoughSpaceToSplit(int lb, int ub, int minRoomLength, int extra = 0);

	/*
	 Generate space from this node.
	 */
	void generate();

	/*
	 Split the space horizontally (creates left and right sections).
	 */
	void generateHorizontalSplit();

	/*
	 Split the space vertically (creates upper and lower sections).
	 */
	void generateVerticalSplit();

	/*
	 Generate a randomly sized room within the space.
	 */
	void generateRoom();

	/*
	 Set the tiles of the dungeon according to the rooms 
	 in the nodes of this tree.
	 */
	void setTiles(Dungeon* dungeon);

	/*
	 Set the floor tiles of the dungeon according to the rooms
	 in the nodes of this tree.
	 */
	void setFloorTiles(Dungeon* dungeon);

	/*
	 Set the wall tiles of the dungeon according to the rooms
	 in the nodes of this tree.
	 */
	void setWallTiles(Dungeon* dungeon);

	/*
	 Get string representation of the node.
	 */
	std::string toString();
};

class BSPTree
{
public:
	void generate(Dungeon* dungeon);
};

#endif
