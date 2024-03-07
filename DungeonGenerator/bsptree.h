#ifndef BSPTREE_H
#define BSPTREE_H

#include <stdio.h>
#include <cstdlib>

#include "node.h"
#include "point.h"
#include "dungeon.h"

// start point is always inclusive
// end point is always exclusive

const int minRoomWidth = 4;
const int minRoomLength = 4;

double randomDouble();

class BSPTree
{
public:
	/*
	 Generate a layout of rooms.
	 @param dungeon: The dungeon to generate for.
	 */
	void generate(Dungeon* dungeon);

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
	void generate(Node& node);

	/*
	 Split the space (creates left and right sections).
	 */
	void generateSplit(bool isVertical, Node& node);

	/*
	 Generate a randomly sized room within the space.
	 */
	void generateRoom(Node& node);

	/*
	 Set the tiles of the dungeon according to the
	 start and end corners of the room.
	 */
	void setTiles(Dungeon* dungeon, Room room);

	/*
	 Set the tiles of the dungeon according to the rooms
	 in the nodes of this tree.
	 */
	void setTiles(Dungeon* dungeon, Node& node);

	/*
	 Set the floor tiles of the dungeon according to the rooms
	 in the nodes of this tree.
	 */
	void setFloorTiles(Dungeon* dungeon, Node& node);

	/*
	 Set the wall tiles of the dungeon according to the rooms
	 in the nodes of this tree.
	 */
	void setWallTiles(Dungeon* dungeon, Node& node);
};

#endif
