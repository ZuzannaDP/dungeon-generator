#include <iostream>

#include "dungeon.h"

// getters and setters
int Dungeon::getWidth() { return width; }
int Dungeon::getLength() { return length; }

void Dungeon::setTile(char type, int x, int y) {
	grid[y * width + x] = type;
}

char Dungeon::getTile(int x, int y) {
	return grid[y * width + x];
}

void Dungeon::print() {
	for (int y = 0; y < length; y++) {
		for (int x = 0; x < width; x++) {
			std::cout.width(3);
			std::cout << getTile(x, y);
		}
		std::cout << "\n";
	}
}
