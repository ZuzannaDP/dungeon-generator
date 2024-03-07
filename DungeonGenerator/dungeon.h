#ifndef DUNGEON_H
#define DUNGEON_H

#include <iostream>
#include <string>

class Dungeon {
private:
	char* grid;
	int width;
	int length;

public:
	Dungeon(int width, int length) {
		this->width = width;
		this->length = length;
		grid = new char[width * length];
	}

	int getWidth();
	int getLength();
};

#endif
