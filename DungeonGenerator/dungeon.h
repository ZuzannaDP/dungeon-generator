#ifndef DUNGEON_H
#define DUNGEON_H

#include <algorithm>
#include <iostream>
#include <string>

const char voidTile = ' ';
const char wallTile = '#';
const char floorTile = '.';

struct Tile 
{
	char type;
};

class Dungeon 
{
private:
	char* grid;
	int width;
	int length;

public:
	Dungeon(int width, int length) {
		this->width = width;
		this->length = length;

		// create grid of given size
		long size = width * length;
		grid = new char[size];

		// fill grid with void
		std::fill(grid, grid + size, voidTile);
	}

	~Dungeon() {
		delete[] grid;
	}

	int getWidth();
	int getLength();

	char getTile(int x, int y);
	void setTile(char type, int x, int y);

	void print();
};

#endif
